import threading, OSC, datetime

colors = ['Blue', 'Green']

class HXMReceiver:
	
	def HXM_handler(self, addr, tags, data, source):
		device = data[0]
		if (len(self.devices) < (device + 1)):
			self.devices.append(HXMWidget())
		self.devices[device].HR=data[1]
		self.devices[device].HRV=data[2]
		self.devices[device].stress = data[3]
		self.devices[device].color = data[4]
		log = {'Timestamp': str(datetime.datetime.now()), 'Player': self.devices[device].color, 'HR':self.devices[device].HR}
		self.hrHistory.append(log)

	def __init__(self, minDevices=1):
		addr = '127.0.0.1', 9000
		self.s = OSC.OSCServer(addr)
		self.s.addMsgHandler("/HXM", self.HXM_handler)
		self.devices = []
		for d in range(minDevices):
			self.devices.append(HXMWidget())
		self.devices[0].HR = 70
		self.hrHistory = []
		#self.devices[1].HR = 81
	
	def run(self):
		self.st = threading.Thread( target = self.s.serve_forever )
		self.st.start()
		
	def close(self):
		self.s.close()
		self.st.join()
		print "Done..."

	def log(self):
		out = "Player,Time,HR\n"
		for item in self.hrHistory:
			out += "{0},{1},{2}\n".format(item['Player'],item['Timestamp'],item['HR'])
		return out

class HXMWidget:

	def __init__ (self):
		self.HR = 0
		self.HRV = 0
		self.stress = 0
		self.color = 'White'