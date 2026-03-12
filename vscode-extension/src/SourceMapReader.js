'use strict';
// ─────────────────────────────────────────────────────────────────────────────
//  SourceMapReader.js
//  Reads a .aero.srcmap JSON file produced by:  aeroc --source-map
//
//  Format:
//  {
//    "version": 1,
//    "aeroFile": "MainActivity.aero",
//    "cppFile":  "MainActivity.cpp",
//    "mappings": [
//      { "aeroLine": 3, "cppLine": 15 },
//      ...
//    ]
//  }
// ─────────────────────────────────────────────────────────────────────────────

const fs = require('fs');
const path = require('path');

class SourceMapReader {
    constructor() {
        /** @type {Map<string, Object>} aeroFilePath → parsed srcmap object */
        this._cache = new Map();
    }

    /**
     * Load (and cache) the source map for a given .aero file.
     * The srcmap file is expected to live next to the .aero file with the
     * extension  <file>.aero.srcmap
     *
     * @param {string} aeroFilePath  absolute path to the .aero source
     * @returns {Object|null}         parsed srcmap or null if not found
     */
    load(aeroFilePath) {
        if (this._cache.has(aeroFilePath)) {
            return this._cache.get(aeroFilePath);
        }

        const srcMapPath = aeroFilePath + '.srcmap';
        if (!fs.existsSync(srcMapPath)) {
            return null;
        }

        try {
            const raw = fs.readFileSync(srcMapPath, 'utf8');
            const map = JSON.parse(raw);
            this._cache.set(aeroFilePath, map);
            return map;
        } catch (e) {
            console.warn(`[AeroLang] Failed to read source map: ${srcMapPath}`, e.message);
            return null;
        }
    }

    /**
     * Reload a source map (call after recompilation).
     * @param {string} aeroFilePath
     */
    invalidate(aeroFilePath) {
        this._cache.delete(aeroFilePath);
    }

    /**
     * Convert a .aero line number to the corresponding generated C++ line.
     * Returns -1 if no mapping found.
     *
     * @param {string} aeroFilePath
     * @param {number} aeroLine  1-based
     * @returns {number}  1-based C++ line, or -1
     */
    aeroToGen(aeroFilePath, aeroLine) {
        const map = this.load(aeroFilePath);
        if (!map) return -1;

        // Find the closest mapping at or before the requested line
        let best = -1;
        let bestDist = Infinity;
        for (const m of map.mappings) {
            if (m.aeroLine <= aeroLine) {
                const dist = aeroLine - m.aeroLine;
                if (dist < bestDist) {
                    bestDist = dist;
                    best = m.cppLine;
                }
            }
        }
        return best;
    }

    /**
     * Convert a generated C++ line number back to the .aero source line.
     * Returns -1 if no mapping found.
     *
     * @param {string} aeroFilePath
     * @param {number} cppLine  1-based
     * @returns {number}  1-based .aero line, or -1
     */
    genToAero(aeroFilePath, cppLine) {
        const map = this.load(aeroFilePath);
        if (!map) return -1;

        let best = -1;
        let bestDist = Infinity;
        for (const m of map.mappings) {
            if (m.cppLine <= cppLine) {
                const dist = cppLine - m.cppLine;
                if (dist < bestDist) {
                    bestDist = dist;
                    best = m.aeroLine;
                }
            }
        }
        return best;
    }

    /**
     * Get the generated .cpp file path for a given .aero file.
     * Returns null if source map not found.
     *
     * @param {string} aeroFilePath
     * @returns {string|null}
     */
    getCppFile(aeroFilePath) {
        const map = this.load(aeroFilePath);
        if (!map) return null;
        // cppFile may be relative — resolve against the directory of the aero file
        if (path.isAbsolute(map.cppFile)) return map.cppFile;
        return path.resolve(path.dirname(aeroFilePath), map.cppFile);
    }
}

module.exports = SourceMapReader;
