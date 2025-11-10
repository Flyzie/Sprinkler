<script setup lang="ts">
import type { PumpInfo } from "../composables/usePump";

const pumpsInfo = defineModel<PumpInfo[]>();
</script>

<template>
  <div class="flex flex-col items-center justify-center gap-1 w-full border-2 border-blue-500 p-1">
    <div
      v-for="pumpInfo in pumpsInfo"
      class="w-full p-2 flex flex-row justify-around items-center bg-blue-500"
    >
      <h1 class="w-full text-3xl">PUMP: {{ pumpInfo.pumpId }}</h1>
      <div class="w-full">
        <p>Water per run: {{ Math.round(pumpInfo.duration / 28) }}ml</p>
        <p>Run every: {{ parseFloat((pumpInfo.cycle / 3600000).toFixed(1)) }}h</p>
      </div>
    </div>
  </div>
</template>
<style scoped></style>
