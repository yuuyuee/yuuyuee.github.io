import flask

app = flask.Flask(__name__)

@app.route("/index", methods=["POST", "GET"])
def Handle():
    return "hello world"

if __name__ == "__main__":
    app.run("0.0.0.0", port=8120, ssl_context=("cert.pem", "key.pem"))

