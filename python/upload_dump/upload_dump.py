import argparse
import serial
import time
import sys

def validate_binary_image(path_to_binary):
    try:
        image = open(path_to_binary, "rb")
    except Exception as e:
        print("Failed to open new image file. Exception: " + str(e) + "\r\n.Exit. (" + path_to_binary + ").")
        exit(-2)
    image_size = len(list(image.read()))
    #print("Image size: " + str(image_size) + " bytes")
    image.close()

def open_serial_port(port, baudrate):
    # start with default baudrate for the bootloader
    try:
        ser = serial.Serial(port, baudrate, timeout=0.1,
                            inter_byte_timeout=0.1)
    except:
        print("Failed to open serial port. Exit.")
        exit(-3)

    if not ser.isOpen():
        print("Failed to open serial port. Exit")
        exit(-3)

    return ser

def get_checksum(data):
    return 18

# I had to workaround the normal serial.write(), as Nordic CLI 
# hangs after sending too long messages.
# So I have to split the data to small pieces, wait while Nordic 
# digestes the received symbols, and proceed afterwards. 
def serial_send(ser, data):
    ser.reset_input_buffer()
    isTransactionComplete = False
    currentIdx = 0
    lastIdx = len(data)
    singleMessageSize = 16
    uart_symbol_duration = 1.0 / 921600
    while not isTransactionComplete:
        nextSize = 0
        if len(data[currentIdx:]) > singleMessageSize:
            nextSize = singleMessageSize
        else:
            nextSize = len(data[currentIdx:])
        ser.write(data[currentIdx:currentIdx + nextSize].encode('utf-8'))
        currentIdx += nextSize
        # time.sleep(uart_symbol_duration * nextSize * 2)
        time.sleep(0.1)
        if nextSize == 0:
            isTransactionComplete = True

    response = ser.read(len(data)).decode('utf-8')
    print(response)
    response = ser.read(5).decode('utf-8')
    # print(response)
    if response.find("ok") < 0:
        print("Bad response: " + response)
    return response

def send_chunk_to_target(ser, offset, size, data):
    message_to_send = "app data "
    if offset == 0:
        message_to_send += "00"
    elif offset < 10:
        message_to_send += '%.2X' % offset
    else:
        message_to_send += hex(offset).lstrip("0x").rstrip("L")
    message_to_send += " "
    message_to_send += '%.2X' % size
    message_to_send += " "
    for byte in data:
        message_to_send += '%.2X' % byte 
    message_to_send += " "
    message_to_send += '%.2X' % get_checksum(data)
    message_to_send += "\r\n"
    serial_send(ser, message_to_send)

    return 0

def upload_binary(ser, path_to_binary):
    chunk_size = 8

    ser.reset_input_buffer()

    # open the image
    image = open(path_to_binary, "rb")

    # send the image to the target, using chunks of a predefined size
    transitionComplete = False
    offset = 0
    transactionResult = 0

    while not transitionComplete:
        nextChunk = bytearray(image.read(chunk_size))
        if len(nextChunk) == 0:
            transitionComplete = True
        else:
            transitionResult = send_chunk_to_target(ser, offset, len(nextChunk), nextChunk)

            offset += len(nextChunk)
        if transactionResult != 0:
            print("Transaction has been interrupted. Exit()")
            exit()

    image.close()

def execute_test_code(ser):
    ser.reset_input_buffer()
    command = "app exec \n"
    command_response = serial_send(ser, command)
    time.sleep(0.5)
    response = ser.read(200).decode('utf-8')
    print((command_response + response).strip()[:-2])


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Tool for uploading executable dump to NRF52-Modularity')
    parser.add_argument('--port', '-p', help='serial port name to open')
    parser.add_argument('--path', '-P', help='path to binary for uploading')
    try:
        args = parser.parse_args()
    except Exception as e:
        print("Failed to parse arguments. Exception: " + str(e))
        print("Exit")
        exit(-3)

    port = vars(args)['port']
    path_to_binary = vars(args)['path']
    baudrate = 921600
    time.sleep(0.1)

    # check if image exists
    validate_binary_image(path_to_binary)

    # open the serial port
    ser = open_serial_port(port, baudrate)

    # upload the binary to the device
    # print("uploading binary")
    upload_binary(ser, path_to_binary)

    # print("executing")
    execute_test_code(ser)

    ser.close()
