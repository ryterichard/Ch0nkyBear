from flask import Flask, request, Response
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import json

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///test.db'
db = SQLAlchemy(app)

class Client(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(80), unique=True, nullable=False)
    date_created = db.Column(db.DateTime, default=datetime.utcnow)

    def __repr__(self):
        return '<Task %r>' % self.id

class Implant(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    computerName = db.Column(db.String(80), nullable=False)
    date_created = db.Column(db.DateTime, default=datetime.utcnow)

    def __repr__(self):
        return '<Task %r>' % self.id

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
    data = Implant.query.all()
    for implant in data:
        print(implant.id, implant.computerName, implant.date_created)
    return Response(status=200, mimetype='application/json')

@app.route('/register', methods=['POST'])
def register():
    data = request.get_json(force=True)
    new_instance = Implant(computerName=data['computerName'])

    try:
        db.session.add(new_instance)
        db.session.commit()
        data = Implant.query.all()
        for implant in data:
            print(implant.id, implant.computerName, implant.date_created)
        return Response(status=201, mimetype='application/json')
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
            print(implant.id, implant.computerName, implant.date_created)
        return Response(status=200, mimetype='application/json')
    except:
        return Response(status=500, mimetype='application/json')

@app.route('/update/<int:id>', methods=['POST'])
def update(id):
    implant_to_update = Implant.query.get_or_404(id)
    data = request.get_json(force=True)
    implant_to_update.computerName = data['computerName']

    try:
        db.session.commit()
        implants = Implant.query.all()
        for implant in implants:
            print(implant.id, implant.computerName, implant.date_created)
        return Response(status=200, mimetype='application/json')
    except:
        return Response(status=500, mimetype='application/json')

if __name__ == "__main__":
    app.run(host="localhost", port=8050, debug=True)