import { api } from "../axios";

export interface PumpInfo {
  pumpId: number;
  duration: number;
  cycle: number;
}

export async function updatePump(pumpInfo: PumpInfo[]): Promise<PumpInfo[]> {
  try {
    const { data } = await api.post("/pumpUpdate", pumpInfo);
    return data;
  } catch (error: any) {
    console.error("API pumpUpdate Error:", error);
    throw error;
  }
}

export async function getPumpsInfo(): Promise<PumpInfo[]> {
  try {
    const { data } = await api.get("/getPumps");
    return data;
  } catch (error: any) {
    console.error("API getPumps Error:", error);
    throw error;
  }
}

export async function pumpNow(pumpNowInfo: Pick<PumpInfo, "pumpId" | "duration">[]) {
  try {
    const { data } = await api.post("/pumpNow", pumpNowInfo);
    return data;
  } catch (error: any) {
    console.error("API pumpNow Error:", error);
    throw error;
  }
}

export async function resetPump(resetInfo: Pick<PumpInfo, "pumpId">[]) {
  try {
    const { data } = await api.post("/resetPump", resetInfo);
    return data;
  } catch (error: any) {
    console.error("API resetPump Error:", error);
    throw error;
  }
}
