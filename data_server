from datetime import datetime
import time
from flask import Flask, request, render_template


app = Flask(__name__)
DATA_FOLDER = "D:/CshsTempData"
row_counts = {
    "cshs_voltage": 1,
    "cshs_voltage_2": 1,
    "cshs_voltage_3": 1,
    "cshs_voltage_4": 1,
    "cshs_voltage_5": 1,
    "cshs_voltage_6": 1,
    "cshs_voltage_7": 1,
    "cshs_voltage_8": 1,
}

def get_file_name(event_name:str, row_ccount: int) -> int:
    if row_ccount <= 2000:
        return f"{DATA_FOLDER}/{event_name}.csv"
    extend = row_ccount // 2000
    return f"{DATA_FOLDER}/{event_name}({extend}.csv)"

@app.route('/')
@app.route('/hello')
def hello():
    return 'hello world'

@app.route("/cshs_voltage",methods=["GET"])
def add_voltage():
    datetime_now = datetime.now()
    timestamp = datetime_now.timestamp()
    event_name = request.args.get('name')
    voltage = request.args.get('v')
    status_code = 0
    message = ''

    for i in range(3):  
        try:
            file_path = get_file_name(event_name, row_counts[event_name])
            f = open(file_path, "a+")
            f.write(f"{event_name},{datetime_now},{timestamp},{voltage}\n")
            f.close()
            row_counts[event_name] += 1
            status_code = 200
            message = f'saved voltage row: [{event_name}: {datetime_now}, {timestamp}, {voltage}]'
            break
        except Exception as ex:
            status_code = 500
            message = "failed to insert, ex: " + str(ex)
        time.sleep(300)
    return message, status_code

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=9000)
