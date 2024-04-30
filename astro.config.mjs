import { fileURLToPath } from "url";
import { dirname, resolve } from "path";
import { defineConfig } from "astro/config";

/**
 * The path of the current module.
 */
const __filename = fileURLToPath(import.meta.url);

/**
 * The path of the directory that contains the current module.
 */
const __dirname = dirname(__filename);

/**
 * The astro configuration.
 */
export default defineConfig({
  vite: {
    resolve: {
      alias: [{ find: "@website", replacement: resolve(__dirname, "src") }],
    },
  },
});
