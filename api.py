from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import os
import uuid

app = Flask(__name__)
CORS(app)

UPLOAD_FOLDER = "uploads"

# create folder if not exists
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

# 🔐 secret key (change this!)
SECRET_KEY = "12345"


# ✅ home
@app.route("/")
def home():
    return "CDFS Secure Backend Running 🔐"


# 🔥 SECURE UPLOAD
@app.route("/upload", methods=["POST"])
def upload():
    key = request.headers.get("key")

    # ❌ unauthorized
    if key != SECRET_KEY:
        return "Unauthorized ❌", 403

    if 'file' not in request.files:
        return "No file ❌", 400

    file = request.files['file']

    if file.filename == "":
        return "No file selected ❌", 400

    # 🔥 random filename (IMPORTANT)
    filename = str(uuid.uuid4()) + "_" + file.filename

    filepath = os.path.join(UPLOAD_FOLDER, filename)
    file.save(filepath)

    return "Uploaded securely 🔥"


# 🔒 BLOCK FILE LIST
@app.route("/files", methods=["GET"])
def list_files():
    return "Access Denied ❌", 403


# 🔓 DOWNLOAD (only if link known)
@app.route("/download/<filename>")
def download_file(filename):
    return send_from_directory(UPLOAD_FOLDER, filename)


# 🚀 run server
if __name__ == "__main__":
    app.run(debug=True)