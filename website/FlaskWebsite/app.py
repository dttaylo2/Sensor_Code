#Import flask, render template
#Import mysql
from flask import Flask, render_template, json, request
from flask.ext.mysql import MySQL
app = Flask(__name__)

#Configure MySQL
mysql = MySQL()
app.config['MYSQL_DATABASE_USER'] = 'root'
app.config['MYSQL_DATABASE_PASSWORD'] = 'Magic331'
app.config['MYSQL_DATABASE_DB'] = 'seniordesign'
app.config['MYSQL_DATABASE_HOST'] = 'localhost'
mysql.init_app(app)

#Define basic route
@app.route('/')
def main():
	return render_template('index.html')

#Define getData method
@app.route('/getData', methods=['GET', 'POST'])
def getData():
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
				'Vibration': row[3],
				'Speed': row[4],
				'Date': row[5]
			}
			data_dict.append(dict_item)

		return json.dumps(data_dict)
	except Exception as e:
		return json.dumps({'error' : str(e)})

#Start application
if __name__ == '__main__':
	app.debug = True
	app.run()