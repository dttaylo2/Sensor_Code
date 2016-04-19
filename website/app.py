#Import flask, render template
#Import mysql
import time
from datetime import datetime, tzinfo, timedelta
from flask import Flask, render_template, json, request
from flask.ext.mysql import MySQL
app = Flask(__name__)

#Configure MySQL
mysql = MySQL()
app.config['MYSQL_DATABASE_USER'] = 'root'
app.config['MYSQL_DATABASE_PASSWORD'] = 'AdamPi331'
app.config['MYSQL_DATABASE_DB'] = 'seniordesign'
app.config['MYSQL_DATABASE_HOST'] = 'localhost'
mysql.init_app(app)

#Define basic route
@app.route('/')
def main():
	return render_template('index.html', date = time.strftime("%B %d, %Y"))

#Define getDates method
@app.route('/getDates', methods=['GET'])
def getDates():
	try:
		#Create connection
		conn = mysql.connect()
		cursor = conn.cursor()

		#Call procedure
		cursor.callproc('sp_getDates', ())

		#Fetch
		data = cursor.fetchall()

		dates_dict = []
		for row in data:
			dict_item = {
				'Date': str(row[0])
			}
			dates_dict.append(dict_item)

		return json.dumps(dates_dict)
	except Exception as e:
		return json.dumps({'error': str(e)})

#Define getChartData method - return 2000 rows
@app.route('/getChartData', methods=['GET', 'POST'])
def getChartData():
	try:
		#Create connection
		conn = mysql.connect()
		cursor = conn.cursor()

		#Call procedure
		cursor.callproc('sp_getAllData', (2000,))

		#Fetch
		data = cursor.fetchall()

		# return json.dumps({'Pulled data!': data})

		#Once we've fetched data, convert to a dictionary
		data_dict = []
		for row in data:
			dict_item = {
				'Id': row[0],
				'Temperature': row[1],
				'Current': row[2],
				'Vibration1': row[5],
				'Vibration2': row[7],
				'Speed': row[3],
				'Date': row[4]
			}
			data_dict.append(dict_item)

		return json.dumps(data_dict)
	except Exception as e:
		return json.dumps({'error' : str(e)})

#Define getTableData method - return 20 rows
@app.route('/getTableData', methods=['GET', 'POST'])
def getTableData():
	try:
		#Create connection
		conn = mysql.connect()
		cursor = conn.cursor()

		#Call procedure
		cursor.callproc('sp_getAllData', (5,))

		#Fetch
		data = cursor.fetchall()

		# return json.dumps({'Pulled data!': data})

		#Once we've fetched data, convert to a dictionary
		data_dict = []
		for row in data:
			dict_item = {
				'Id': row[0],
				'Temperature': row[1],
				'Current': row[2],
				'Vibration1': row[5],
				'Vibration2': row[7],
				'Speed': row[3],
				'Date': row[4],
                                'Classification': row[9]
			}
			data_dict.append(dict_item)

		return json.dumps(data_dict)
	except Exception as e:
		return json.dumps({'error' : str(e)})

#Start application
if __name__ == '__main__':
	app.debug = True
	app.run()
