"""

    minify.py
    Little script for minifying and gzipping web files

    Marco Volpato 2018

"""

import os
import gzip
import json
import binascii
import textwrap

from jsmin import jsmin
from css_html_js_minify import css_minifier, html_minifier

INPUT_DIR = '../../web_interface/'
OUTPUT_FILE = '../../SomfyControl/webfiles.h'
TYPES = {
    'lang': 'application/json',
    'html': 'text/html',
    'json': 'application/json',
    'css': 'text/css',
    'js': 'application/javascript'
}
EXCLUSIONS = ['devices.json', 'config.json']

progmem_definitions = ''
webserver_rules = ''

def read_file(file_name):
    f = open(INPUT_DIR + file_name, 'r')
    data = f.read()
    f.close()
    return data

def minify_json(data):
    minified = json.dumps(json.loads(data), separators=(',', ':'))
    return minified

def compress(data):
    compressed = gzip.compress(str.encode(data))
    return compressed

def minify(files):
    global progmem_definitions, webserver_rules

    for f in files:

        if f not in EXCLUSIONS:
            minified = ''
            type_name = ''

            if f.endswith('.html'):
                print('[+] Minifying ' + f + '...')
                minified = html_minifier.html_minify(read_file(f))
                type_name = 'Page'
            elif f.endswith('.css'):
                print('[+] Minifying ' + f + '...')
                minified = css_minifier.css_minify(read_file(f))
                type_name = 'Style'
            elif f.endswith('.js'):
                print('[+] Minifying ' + f + '...')
                minified = jsmin(read_file(f))
                type_name = 'Script'
            elif f.endswith('.json'):
                print('[+] Minifying ' + f + '...')
                minified = minify_json(read_file(f))
                type_name = 'Json'
            elif f.endswith('.lang'):
                print('[+] Minifying ' + f + '...')
                minified = minify_json(read_file(f))
                type_name = 'Lang'

            if minified is not '':
                name = f.split('.')[0]
                try:
                    name = name.split('/')[1]
                except:
                    pass
                name = name + type_name
                hex_formatted_content = ''

                print('[+] Compressing ' + f + '...')

                hex_content = binascii.hexlify(compress(minified))
                hex_content = hex_content.decode('UTF-8')
                hex_content = [hex_content[i:i+2]
                               for i in range(0, len(hex_content), 2)]

                for char in hex_content:
                    hex_formatted_content += '0x' + char + ', '

                hex_formatted_content = hex_formatted_content[:-2]
                progmem_definitions += 'const char ' + name + \
                    '[] PROGMEM = {' + hex_formatted_content + '};\n'
                rule = """
                server.on("/""" + f + """\", []() {
                    sendProgmem(""" + name + """, sizeof(""" + name + """), \"""" + TYPES[f.split('.')[1]] + """\");
                });
                """
                webserver_rules += textwrap.dedent(rule)


if __name__ == '__main__':
    files = os.listdir(INPUT_DIR)
    minify(files)

    files = os.listdir(INPUT_DIR + 'langs/')
    langs = []
    for f in files:
        lang = 'langs/' + f
        langs.append(lang)
    minify(langs)


    print('[+] Creating web files header...')
    output = open(OUTPUT_FILE, 'w')
    output.write(progmem_definitions)
    output.close()
    print('\n')

    print('[+] Web Server rules:')
    print(webserver_rules)
