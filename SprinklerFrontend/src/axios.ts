import axios from "axios";

export const api = axios.create({
  baseURL: "http://sprinkler.local:8080/",
  timeout: 1000,
  headers: { "Content-Type": "application/json" },
});
