import { GoogleGenerativeAI } from '@google/generative-ai';
import dotenv from 'dotenv';
dotenv.config();

async function teste() {
  const API_KEY = process.env.GEMINI_API_KEY;
  const genAI = new GoogleGenerativeAI(API_KEY);
  const model = genAI.getGenerativeModel({ model: 'models/text-bison-001' });

  const prompt = "Escreva uma frase curta e motivacional.";

  const result = await model.generateContent({ prompt });
  console.log('Resposta:', result.response.text());
}

teste().catch(console.error);
