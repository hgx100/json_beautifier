# JSON Beautifier

A simple CLI tool to format JSON files with indentation. Uses C++23, `nlohmann/json`, and `CLI11`. Probably overkill, but whatever.

## Building
```bash
git clone https://github.com/hgx100/json_beautifier.git
cd json_beautifier
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage
```bash
./json_beautifier <input_file> [-i <indent>] [-o <output_file>]
```
- `-i` (optional) – spaces for indentation (default: 4)
- `-o` (optional) – output file name (default: `input_beautified.json`)

If the output file exists, it'll ask before overwriting. Also prints how long the conversion took (not including file I/O). 

### Example
```bash
./json_beautifier data.json --indent 2 --output pretty.json
```


