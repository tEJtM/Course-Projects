oifrom Tkinter import *
import socket
global sock
global s1, s2, s3, s4
def sendMail() :#when you press the "Send" button
	global sock
	global s1, s2, s3, s4
	sock.send('HELO ' + s3.get() + '\n')
	sock.send('MAIL FROM: <' + s1.get() + '>\n')
	sock.send('RCPT TO: <' + s2.get() + '>\n')
	sock.send('DATA\n')
	sock.send(e4.get(1.0, END))
	sock.send('\n.\n') # send the mail under STMP protocol
	return
def retrMail() :#when you press the "Retr" button to retrieve a mail
	global sock, s1, ps, idx, m1
	sock.send('USER ' + s1.get() + '\n')#claim your identity
	sock.send('PASS ' + ps.get() + '\n')
	sock.send('RETR ' + idx.get() + '\n')
	st = ""
	lst1 = 'a'
	lst2 = 'a'
	buf = 'a'
	while True:#receiving the mail from the server
		lst2 = lst1
		lst1 = buf
		buf = sock.recv(1)
		if (lst2 == '\n' or lst2 == '\r') and (buf == '\n' or buf == '\r') and lst1 == '.':
			break
		else :
			st = st + buf
	m1.set(st)
	return
def listMail() :#when you press the "List" button to see the number of your mails
	global sock, s1, ps, m1
	sock.send('USER ' + s1.get() + '\n')#claim your identity
	sock.send('PASS ' + ps.get() + '\n')
	sock.send('LIST\n')
	st = ""
	while True:#receiving the message from the server
		buf = sock.recv(1)
		if buf != '.' :
			st += buf
		else :
			break
	m1.set(st)
	return
def deleMail() :#when you press the "Dele" button to delete one of your mails
	global sock, s1, ps, idx
	sock.send('USER ' + s1.get() + '\n')#claim your identity
	sock.send('PASS ' + ps.get() + '\n')
	sock.send('DELE ' + idx.get() + '\n')
	while True :
		buf = sock.recv(1)
		if buf == '.' :
			break
	return
def ex() :#when you press the "Exit" button to exit
	global sock
	sock.send('EXIT\n')
global sock
global s1, s2, s3, e4, ps, idx, mailText, m1
master = Tk()
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1', 7073))#connect to the server at 127.0.0.1, 7073. The IP address and the port number can be changed.
Label(master, text="From").grid(row=0)#the GUI
Label(master, text="To").grid(row=1)
Label(master, text="Name").grid(row = 2)
Label(master, text="Data").grid(row=3)
Label(master, text = "password").grid(row = 0, column = 2)
ps = Entry(master)
ps.grid(row = 0, column = 3)
Label(master, text = "index").grid(row = 1, column = 2)
idx = Entry(master)
idx.grid(row = 1, column = 3)
s1 = StringVar()
s2 = StringVar()
s3 = StringVar()

e1 = Entry(master, textvariable = s1)
e2 = Entry(master, textvariable = s2)
e3 = Entry(master, textvariable = s3)
e4 = Text(master)

e1.grid(row=0, column=1)
e2.grid(row=1, column=1)
e3.grid(row=2, column = 1)
e4.grid(row=3, column = 1)
m1 = StringVar()
Button(master, text = "Send", command = sendMail).grid(row = 4, column = 1)
mailText = Label(master, textvariable = m1)
mailText.grid(row = 3, column = 2)
Button(master, text = "list", command = listMail).grid(row = 2, column = 2)
Button(master, text = "retr", command = retrMail).grid(row = 4, column = 2)
Button(master, text = "dele", command = deleMail).grid(row = 5, column = 2)
Button(master, text = "EXIT", command = ex).grid(row = 5, column = 1)
mainloop()
