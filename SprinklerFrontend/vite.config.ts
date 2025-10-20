import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
import tailwindcss from "@tailwindcss/vite";

// https://vite.dev/config/
export default defineConfig({
  base: "./",
  define: {
    __VUE_OPTIONS_API__: false,
    __VUE_PROD_DEVTOOLS__: false,
  },
  plugins: [vue(), tailwindcss()],
  build: {
    assetsDir: ".",
    cssCodeSplit: false,
    sourcemap: false,
    rollupOptions: {
      treeshake: {
        moduleSideEffects: false,
      },
      output: {
        entryFileNames: "index.js",
        chunkFileNames: "index.js",
        assetFileNames: (assetInfo) => {
          if (assetInfo.name && assetInfo.name.endsWith(".css"))
            return "index.css";
          return "[name][extname]";
        },
        manualChunks: undefined,
      },
    },
    minify: "terser",
    terserOptions: {
      compress: {
        drop_console: true,
        drop_debugger: true,
        passes: 2,
        unsafe: true,
        pure_getters: true,
      },
      mangle: {
        toplevel: true,
      },
      format: {
        comments: false,
      },
    },
    target: "es2017",
  },
});
