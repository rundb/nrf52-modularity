
import re
import argparse
import os
import subprocess
import sys


def log_e(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def run_utility(cmd):
    result = subprocess.run(cmd, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    err_output = result.stderr.decode('utf-8')
    return result.stdout.decode('utf-8'), err_output if len(err_output) > 0 else None


def extract_function_binary_code(function_name, dump):
    """
    Extracts the code of the first function that matches the mangled 'function_name'
    for c++ or unique function for c
    """
    pattern = re.compile('\\d+ <.*{}.*>:'.format(function_name))
    dump_lines = dump.splitlines()

    function_header_index = next(
        (i for i, item in enumerate(dump_lines) if pattern.match(item)), None)
    if function_header_index is None:
        return []

    byte_code = []
    for line in dump_lines[function_header_index + 1:]:
        if line == '':
            break

        line_segments = line.split('\t')
        code_string = line_segments[1].replace(' ', '')
        byte_code.extend([int(code_string[i:i+2], 16)
                          for i in range(0, len(code_string), 2)])

    return byte_code


def check_prerequisites(required_utils, source_file, verbose):
    from shutil import which

    if verbose:
        passed = True
        for u in required_utils:
            if which(u):
                print(u + " found")
            else:
                print(u + " not found")
                passed = False

        if not os.path.exists(source_file):
            print("source file " + u + " not found")
            passed = False

        return passed

    else:
        return all(which(u) for u in required_utils) and os.path.exists(source_file)


def format_dump_as_c_array(function_name, dumped_values):
    dump = 'char {name}[{length}] = {{ {values} }};'.format(
        name=function_name,
        length=len(dumped_values),
        values='0x' + ', 0x'.join("{:02x}".format(i) for i in dumped_values))
    return dump


def format_dump_as_hex_stream(dumped_values):
    return ''.join("{:02x}".format(i) for i in dumped_values)


def parse_args():
    arg_parser = argparse.ArgumentParser(
        description='Utility to extract binary code')
    arg_parser.add_argument('source', help='c source file to process')
    arg_parser.add_argument(
        'function', help='function which code to extract (currently the first found overload will be used)')

    arg_parser.add_argument('-t', '--tooling_prefix',
                            help='prefix for gnu tooling used to process code (can contain toolchain path)', default='')

    arg_parser.add_argument(
        '-f', '--gcc_flags', help='parameters that should be forwarded to gcc', default='')

    arg_parser.add_argument(
        '-o', '--output', help='file path to output the dump', default=None)

    arg_parser.add_argument(
        '-c', '--c_array', help='output dump as c array', action='store_true')

    arg_parser.add_argument(
        '-v', '--verbose', help='verbose output', action='store_true')

    args = arg_parser.parse_args()

    return args


if __name__ == '__main__':
    args = parse_args()

    gcc = args.tooling_prefix + 'gcc'
    obj_dump = args.tooling_prefix + 'objdump'

    if not check_prerequisites([gcc, obj_dump], args.source, args.verbose):
        log_e("Error: the required utilities can not be found in the path or source file doesn't exist")
        exit(1)

    gcc_compile_cmd = [
        gcc] + args.gcc_flags.split(' ') + ['-o', args.source + '.o', '-c'] + [args.source]

    out, err = run_utility(gcc_compile_cmd)
    if err is not None:
        log_e(err)
        exit(1)

    obj_dump_output, err = run_utility([obj_dump, '-dw', '-j', '.text', args.source + '.o'])
    if err is not None:
        log_e(err)
        exit(1)

    function_dump = extract_function_binary_code(args.function, obj_dump_output)
    if len(function_dump) == 0:
        log_e("Error: the code for the specified function was not found")
        exit(1)   

    if args.output is not None:
        with open(args.output, 'wb') as f:
            f.write(bytearray(function_dump))

    output = format_dump_as_hex_stream(
        function_dump) if not args.c_array else format_dump_as_c_array(args.function, function_dump)
    print(output)

    exit(0)
