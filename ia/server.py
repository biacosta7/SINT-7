# # server.py
# from flask import Flask, request, jsonify
# import google.generativeai as genai

# app = Flask(__name__)
# genai.configure(api_key="AIzaSyCRcuHRmukhpibCQOkdATpEQ0-s0eRgp-Y")

# model = genai.GenerativeModel("gemini-pro")

# @app.route('/gerar_fragmento', methods=['POST'])
# def gerar_fragmento():
#     dados = request.json
#     fase = dados.get("fase")
#     sentimento = dados.get("sentimento")

#     prompt = f"Gere um fragmento de memória com sentimento {sentimento}, ambientado na fase {fase}."
#     response = model.generate_content(prompt)

#     return jsonify({"conteudo": response.text})

# if __name__ == '__main__':
#     app.run(port=5000)

from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/gerar', methods=['POST'])
def gerar():
    data = request.get_json()
    pergunta = data.get("pergunta", "")
    resposta = f"Você perguntou: {pergunta}"
    return jsonify(resposta=resposta)

if __name__ == "__main__":
    app.run(debug=True)
