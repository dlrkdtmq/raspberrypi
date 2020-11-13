#controlServer.py
import cgi
import RPi.GPIO as gpio
from BaseHTTPServer import HTTPServer
from SimpleHTTPServer import SimpleHTTPRequestHandler
LED = 23
class Handler(SimpleHTTPRequestHandler):
  def do_POST(self):
    
    if self.path == '/home/pi/ws/web-control':
      form = cgi.FieldStorage(fp = self.rfile, headers = self.headers, environ = {'REQUEST_METHOD':'POST'})
      cmd = form['command'].value
      print(cmd)
      
      if cmd == "ON":
        gpio.output(LED, gpio.HIGH)
      elif cmd == "OFF" :
        gpio.output(LED, gpio.LOW)
      
      self.send_response(100)
      self.send_header('Content-type', 'text/html')
      return

    return self.do_GET()
    
gpio.cleanup()
gpio.setmode(gpio.BCM)
gpio.setup(LED, gpio.OUT)

server = HTTPServer(('',8002), Handler).serve_forever()
