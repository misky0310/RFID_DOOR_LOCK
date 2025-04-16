from flask import Flask, render_template, request, redirect, flash, url_for
from pymongo import MongoClient
from datetime import datetime
import os

app = Flask(__name__)
app.secret_key = os.environ.get('SECRET_KEY','default_secret_key')

client = MongoClient(os.environ.get('MONGODB_URI', 'mongodb://localhost:27017/'))
db = client['rfid_db']
collection = db['authorized_cards']

@app.route('/')
def home():
    tags = list(collection.find())
    return render_template('index.html', tags=tags)

@app.route('/add', methods=['POST'])
def add_card():
    uid = request.form['uid'].strip().upper()
    owner = request.form['owner'].strip()

    if not collection.find_one({'uid': uid}):
        collection.insert_one({'uid': uid, 'owner': owner,'createdAt': datetime.now()})
        flash('RFID tag added successfully!', 'success')
    else:
        flash('This RFID UID already exists in the database.', 'error')

    return redirect(url_for('home'))

if __name__ == '__main__':
    app.run(debug=True)
