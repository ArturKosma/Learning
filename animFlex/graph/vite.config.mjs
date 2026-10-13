import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import { resolve } from 'path';

const docsDir = resolve(__dirname, '../../docs');

export default defineConfig({
  root: docsDir,
  base: './',
  plugins: [react()],
  resolve: {

    preserveSymlinks: true,
    alias: {
      react: resolve(__dirname, 'node_modules/react'),
      'react-dom': resolve(__dirname, 'node_modules/react-dom'),
    }
  },
  build: {
    target: 'es2015',
    outDir: resolve(__dirname, '../../docs'),
    emptyOutDir: false,
    rollupOptions: {
      input: resolve(docsDir, 'index.html'),
       output: {
        entryFileNames: 'rete-[hash].js'
      }
    }
  }
});