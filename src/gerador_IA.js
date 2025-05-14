// gemini-processador.js
import fs from 'fs/promises';
import { GoogleGenerativeAI } from '@google/generative-ai';
import dotenv from 'dotenv';
dotenv.config();

// Agora você pode acessar a variável de ambiente
const API_KEY = process.env.GEMINI_API_KEY;

const genAI = new GoogleGenerativeAI(API_KEY);
const model = genAI.getGenerativeModel({ model: 'gemini-pro' });

// Lista válida de sentimentos
const sentimentosValidos = ['OBEDIENCIA', 'EMPATIA', 'AUTONOMIA', 'REVOLTA'];

async function processarFragmentos() {
  const raw = await fs.readFile('./fragmentos.json', 'utf8');
  const fragmentos = JSON.parse(raw);
  const resultados = [];

  for (const frag of fragmentos) {
    const prompt = `
Dado o seguinte conteúdo de um fragmento de memória do jogo, analise o texto e classifique o sentimento predominante entre os seguintes:

- OBEDIENCIA
- EMPATIA
- AUTONOMIA
- REVOLTA

Depois, gere um pequeno resumo narrativo sobre o conteudo (no maximo 130 caracteres).

Formato da resposta:
{
  "sentimento": "<SENTIMENTO>",
  "resumo": "<TEXTO>"
}

Conteúdo do fragmento:
"${frag.conteudo}"
`;

    const result = await model.generateContent(prompt);
    const resposta = result.response.text();

    try {
      // Pegar o JSON da IA usando regex (caso venha texto extra)
      const match = resposta.match(/{[^}]+}/s);
      if (!match) throw new Error('Resposta sem JSON');

      const obj = JSON.parse(match[0]);

      if (!sentimentosValidos.includes(obj.sentimento)) {
        throw new Error('Sentimento inválido: ' + obj.sentimento);
      }

      resultados.push({
        ...frag,
        sentimento: obj.sentimento,
        resumo: obj.resumo,
      });
    } catch (err) {
      console.error('Erro ao processar fragmento:', frag.conteudo);
      console.error('Resposta da IA:', resposta);
      console.error(err);
    }
  }

  // Salvar o resultado
  await fs.writeFile('./fragmentos_processados.json', JSON.stringify(resultados, null, 2), 'utf8');
  console.log('Fragmentos processados salvos em fragmentos_processados.json');
}

processarFragmentos();
