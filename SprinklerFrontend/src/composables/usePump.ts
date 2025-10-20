import { api } from "../axios";

export interface PumpInfo {
  pumpId: number;
  duration: number;
  cycle: number;
}

export async function updatePump(pumpInfo: PumpInfo[]): Promise<PumpInfo[]> {
  const payload = pumpInfo.map((pump) => {
    return { pump: pump.pumpId, duration: pump.duration, cycle: pump.cycle };
  });

  try {
    const { data } = await api.post("/pumpUpdate", { data: payload });
    return data;
  } catch (error: any) {
    console.error("API Error:", error);
    throw error;
  }
}
