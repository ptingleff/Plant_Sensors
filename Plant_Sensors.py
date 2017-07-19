import http.server
import time
import glob
import sys
import serial
import threading

ser = serial.Serial("/dev/ttyACM0",9600)
ser.flush()
address = ('localhost',8000)

def makeLine():
    line=[]
    while len(line) != 9:
        f = open('/home/pi/output.txt','r+')
        data = ser.readline().decode().replace('\r\n','')
        if data:
            line.append(data)
    else:
        soil = line[0]
        infrared = line[1]
        full = line[2]
        visible = line[3]
        lux = line[4]
        temp = line[5]
        pressure = line[6]
        altitude = line[7]
        humidity = line[8]
        line1 = str("Soil Moisture: " + soil + " Infrared: " + infrared + " Full Light: " + full + " Visible Light: " + visible + " Lux: " + lux + " Temperature: " + temp + " Pressure: " + pressure + " Altitude: " + altitude + " Humidity: " + humidity)
        line=[]
        return line1

##def loopMakeLine():
##    threading.Timer(10.0,makeLine()).start()
##    return makeLine()

class WebHandler(http.server.BaseHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header("Content-type","text/html")
        self.end_headers()
    def do_GET(self):
        #if self.path.endswith('get_data'):
            self.send_response(200)
            self.send_header('Content-type','text/html')
            self.end_headers()
            self.wfile.write(bytes("<html><head><title>PlantPI**</title></head>",'utf-8'))
            self.wfile.write(bytes("<body><p>",'utf-8'))
            self.wfile.write(bytes(makeLine(),'utf-8'))
            self.wfile.write(bytes("</p></body></html>",'utf-8'))
            self.wfile.close()

if __name__ == '__main__':
    dataAPI = http.server.HTTPServer(address,WebHandler)
    try:
        dataAPI.serve_forever()
    except KeyboardInterrupt
        print("Closing Socket.")
        dataAPI.socket.close()
        exit()
