from flask import Flask, request, jsonify
from flask_cors import CORS
import os

# app create
app = Flask(__name__)

# enable CORS (VERY IMPORTANT)
CORS(app)

# upload folder
UPLOAD_FOLDER = "uploads"

# create folder if not exists
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

# home route
@app.route("/")
def home():
    return "CDFS Backend Running 🚀"

# upload route
@app.route("/upload", methods=["POST"])
def upload():
    if 'file' not in request.files:
        return "No file received ❌", 400

    file = request.files['file']

    if file.filename == "":
        return "No file selected ❌", 400

    filepath = os.path.join(UPLOAD_FOLDER, file.filename)
    file.save(filepath)

    return f"File '{file.filename}' uploaded successfully 🔥"

# list files (optional but useful)
@app.route("/files", methods=["GET"])
def list_files():
    files = os.listdir(UPLOAD_FOLDER)
    return jsonify(files)

# run server
if __name__ == "__main__":
    app.run(debug=True)