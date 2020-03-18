import json
file_name = 'parameter.json'

def read_parameter():
    with open(file_name,'r') as fp:
        return json.loads(fp.read())

def save_parameter(*parameter):
    with open(file_name, 'w') as fp:
        fp.write(json.dumps(parameter))
