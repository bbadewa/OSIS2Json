# OSIS to JSON Converter

## What is this?
OSIS to JSON is a command-line tool created to facilitate the conversion of bibles in .XML Open Scripture Information Standard (OSIS) format to .json files, for use with [Nathan Patton's PythonBible](https://github.com/avendesora/pythonbible) or [Kuwagata/Kuwagata4OBS.](https://github.com/bbadewa/Kuwagata4OBS)

## Usage
```osis2json -i [.xml file in OSIS format] -o [destination] -t [number of threads]```

For larger OSIS files, or on machines with superior multi-threading performance, you'll want to use ``-t`` to allow the tool to take advantage of your hardware to convert files in seconds rather than minutes. 
