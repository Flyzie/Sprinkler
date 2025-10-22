<script setup lang="ts">
import { computed, ref, watch } from "vue";
import PumpButton from "./components/pumpButton.vue";
import { updatePump } from "./composables/usePump";
import PumpsInfo from "./components/pumpsInfo.vue";

const maxPumpAmount = 2000;

const pumps = ref([
  { id: 0, active: false },
  { id: 1, active: false },
]);

const pumpAmount = ref<number>(0);
const cycleHour = ref<number>(0);
const cycleDay = ref<number>(0);

const updateSucces = ref<boolean>(false);

const pumpLabel = computed(() => {
  return pumps.value.length > 1 ? "Pumps" : "Pump";
});

const handleUpdatePumps = async () => {
  const activePumps = pumps.value
    .filter((pump) => pump.active)
    .map((pump) => ({
      pumpId: pump.id,
      duration: pumpAmount.value * 28,
      cycle: (cycleDay.value * 24 + cycleHour.value) * 60 * 1000,
    }));

  if (activePumps.length === 0) {
    alert("Please select at least one pump");
    return;
  }

  try {
    await updatePump(activePumps);
    updateSucces.value = true;
  } catch (error) {
    console.error("Failed to update pumps", error);
  }
};

const toggleDisabled = computed(() => {
  if (pumpAmount.value === 0 || cycleDay.value + cycleHour.value === 0) {
    return true;
  } else {
    return false;
  }
});

watch(cycleHour, (newValue) => {
  if (newValue < 0) {
    cycleHour.value = 0;
    if (cycleDay.value > 0) {
      cycleHour.value = 23;
      cycleDay.value--;
    }
  } else if (newValue > 23) {
    cycleHour.value = 0;
    cycleDay.value++;
  }
});

const incrementHour = () => {
  cycleHour.value++;
};

const decrementHour = () => {
  cycleHour.value--;
};

const barProgress = computed(() => {
  let barPrecentage = (pumpAmount.value / maxPumpAmount) * 100;

  return `linear-gradient(to right, #3b82f6 ${barPrecentage}%, #16245650 ${barPrecentage}%)`;
});
</script>
<template>
  <form
    @submit.prevent="handleUpdatePumps"
    class="flex flex-col items-start justify-start w-full gap-6"
  >
    <div class="flex flex-col w-full items-start justify-center">
      <h1 class="text-4xl">Water every:</h1>
      <div class="flex flex-row w-full items-center">
        <input placeholder="0" type="number" min="0" max="24" v-model="cycleDay" class="text-7xl" />
        <h1 class="text-7xl">Days</h1>
      </div>
      <div class="flex flex-row w-full items-center">
        <input
          placeholder="0"
          type="number"
          min="0"
          max="24"
          v-model="cycleHour"
          class="text-7xl"
        />
        <h1 class="text-7xl">Hr's</h1>
        <div class="flex flex-row gap-2 w-full justify-end">
          <button
            type="button"
            @click="incrementHour"
            class="text-8xl cursor-pointer text-blue-500"
          >
            +
          </button>
          <button type="button" @click="decrementHour" class="text-8xl cursor-pointer">-</button>
        </div>
      </div>
    </div>
    <input
      v-model="pumpAmount"
      type="range"
      min="0"
      :max="maxPumpAmount"
      :style="{ background: barProgress }"
      class="w-full h-4 rounded-none appearance-none cursor-pointer slider bg-blue-950"
    />
    <h1 class="text-md">Water to pump: {{ pumpAmount }} ml</h1>
    <div class="flex flex-row w-full gap-4">
      <PumpButton
        v-for="pump in pumps"
        :key="pump.id"
        :pump-id="pump.id"
        @activated="pump.active = !pump.active"
      ></PumpButton>
    </div>
    <div class="flex flex-col gap-2 w-full">
      <button
        type="submit"
        :disabled="toggleDisabled"
        class="w-full p-6 text-4xl cursor-pointer active:border-4 active:border-solid"
        :class="toggleDisabled ? 'bg-blue-500/20' : 'bg-blue-500'"
      >
        Update {{ pumpLabel }}
      </button>

      <button
        type="submit"
        :disabled="toggleDisabled"
        class="w-full p-2 text-4xl cursor-pointer active:border-4 active:border-solid"
        :class="toggleDisabled ? 'bg-blue-500/20' : 'bg-blue-500'"
      >
        Pump Now
      </button>
    </div>
    <h1 v-if="updateSucces">Pumps Updated succesfully!</h1>
    <PumpsInfo></PumpsInfo>
  </form>
</template>

<style scoped>
input[type="number"] {
  width: 3ch;
  padding: 0;
  margin: 0;
  border: none;
  background: transparent;
  outline: none;
}

input[type="range"].slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 30px;
  height: 30px;
  background: var(--color-blue-500);
  cursor: pointer;
  border: 2px solid #fff;
}

input[type="range"].slider::-moz-range-thumb {
  width: 30px;
  height: 30px;
  background: var(--color-blue-500);
  cursor: pointer;
  border-radius: 50%;
  border: 2px solid #fff;
}
</style>
