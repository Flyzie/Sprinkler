<script setup lang="ts">
import { ref } from "vue";
import type { PumpInfo } from "../composables/usePump";

const pumpsInfo = ref<PumpInfo[]>([]);
</script>
<template>
  <div v-for="pumpInfo in pumpsInfo"></div>
</template>
<style scoped></style>
