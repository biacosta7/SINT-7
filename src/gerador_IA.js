import fs from 'fs/promises';
import { GoogleGenerativeAI } from '@google/generative-ai';
import dotenv from 'dotenv';
dotenv.config();

const API_KEY = process.env.GEMINI_API_KEY;
const genAI = new GoogleGenerativeAI(API_KEY);
const model = genAI.getGenerativeModel({ model: "models/gemini-1.5-pro-latest" });

async function extrairFragmentos() {
  const raw = await fs.readFile('./SINT-7/src/historia.json', 'utf8');



  const { historia } = JSON.parse(raw);

  const prompt = `
Você é um roteirista de jogos. Abaixo está a história do jogo. Extraia de 3 a 5 fragmentos emocionais curtos (1-2 frases no máximo).

Para cada fragmento, classifique com um dos seguintes sentimentos:

- OBEDIENCIA
- EMPATIA
- AUTONOMIA
- REVOLTA

Formato da resposta:
[
  {
    "conteudo": "...",
    "sentimento": "..."
  },
  ...
]

História:
${historia}
`;

  const result = await model.generateContent(prompt);
  const resposta = result.response.text();

  try {
    const match = resposta.match(/\[\s*{[\s\S]+}\s*\]/);
    if (!match) throw new Error("Não foi possível extrair JSON");

    const fragmentos = JSON.parse(match[0]);

    await fs.writeFile('./fragmentos_processados.json', JSON.stringify(fragmentos, null, 2));
    console.log('Fragmentos gerados salvos em fragmentos_processados.json');
  } catch (err) {
    console.error('Erro ao processar a resposta da IA:');
    console.error(resposta);
    console.error(err);
  }
}

extrairFragmentos();