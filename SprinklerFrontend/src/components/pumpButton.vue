<script setup lang="ts">
import { ref } from "vue";

const props = defineProps<{ pumpId: number }>();

const emit = defineEmits(["activated"]);
const isActive = ref(false);

const changeButtonState = () => {
  isActive.value = !isActive.value;
  emit("activated", { pumpId: props.pumpId, isActive: isActive.value });
};
</script>

<template>
  <button
    type="button"
    @click="changeButtonState"
    class="w-full text-2xl p-3 transition-colors"
    :class="isActive ? 'bg-emerald-500' : 'bg-indigo-500'"
  >
    {{ isActive ? "PUMP" : "PUMP" }}: {{ pumpId }}
  </button>
</template>

<style scoped></style>
