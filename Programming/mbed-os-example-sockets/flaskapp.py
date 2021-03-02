import json
from flask import Flask, request

app = Flask(__name__)


@app.route('/')
def hello_world():
    return 'Hello World!'


@app.route('/temperature',   methods=['POST'])
def get_temperature():
    content = request.get_json()
    print(content)
    return content




if __name__ == '__main__':
    app.run()