import json
from flask import Flask, request, jsonify

app = Flask(__name__)

number_of_requests = 0
@app.route('/')
def hello_world():
    return 'Hello World!'

@app.route('/temperature',   methods=['POST'])
def get_temperature():
    content = request.get_json()
    print(content)
    return content

@app.route('/LED', methods=['GET'])
def turn_on_led():
    global number_of_requests
    if number_of_requests % 2 == 0:
        led_state = {
            'led_state': '1'
        }
        number_of_requests = number_of_requests + 1
    else:
        led_state = {
            'led_state': '0'
        }
        number_of_requests = number_of_requests + 1

    return jsonify({'led_state': led_state}), 201

if __name__ == '__main__':

    app.run()