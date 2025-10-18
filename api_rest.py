from flask import Flask, request, jsonify
import time

app = Flask(__name__)

dados = []
ultimo_tempo = 0  # guarda a última vez que recebeu um POST

@app.route('/', methods=['GET'])
def home():
    return "API Simples na Raspberry Pi"

@app.route('/dados', methods=['GET'])
def listar_dados():
    return jsonify(dados)

@app.route('/dados', methods=['POST'])
def adicionar_dado():
    global ultimo_tempo
    agora = time.time()

    if agora - ultimo_tempo < 60:  # se não passou 1 minuto
        return jsonify({"mensagem": "Espere 1 minuto antes de enviar outro dado!"}), 429

    novo_dado = request.get_json()
    dados.append(novo_dado)
    ultimo_tempo = agora  # atualiza o tempo do último POST
    return jsonify({"mensagem": "Dado adicionado com sucesso!", "dado": novo_dado}), 201

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
