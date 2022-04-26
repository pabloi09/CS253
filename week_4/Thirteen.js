//Continuation-passing style

const fs = require('fs')

function checkInput(args, obj){
  if (args.length != 4){
    console.log("Please, as an input, provide:\n1. The stop words file \n2.The file to analyse")
    obj['data'] = false
  }
}

function readStopWords(file, obj){
  obj['data'] = new Set(fs.readFileSync(file, 'utf-8').split(','))
}

function readInputFile(file, obj){
  obj['data'] = fs.readFileSync(file, 'utf-8').replace(/[\W_]+/g," ").toLowerCase().split(" ")
}

function getFrequencies(wordCount){
  freqs = {}
  maxFreq = 0
  for (word in wordCount){
    freq = wordCount[word]
    if (!freq[freqs]) freqs[freq] = new Set()
    
    freqs[freq].add(word)
    maxFreq = Math.max(maxFreq, freq)
  }
  return [freqs, maxFreq]
}

input_checker_obj = {
  'data': true,
  'init': (args) =>  checkInput(args, input_checker_obj),
  'is_valid': () => input_checker_obj['data']
}

stop_words_obj = {
  'data' : [],
  'init' : (args) => readStopWords(args[2], stop_words_obj),
  'is_stop_word': (word) => (word.length <= 1 || stop_words_obj['data'].has(word))
}

data_storage_obj = {
  'data' : [],
  'init': (args) => readInputFile(args[3], data_storage_obj),
  'words': () => data_storage_obj['data']
}

word_freqs_obj = {
  'data' : {},
  'increment_count': (word) => word_freqs_obj['data'][word] = word_freqs_obj['data'][word] ? word_freqs_obj['data'][word]+1 : 1,
  'freqs': () => getFrequencies(word_freqs_obj['data'])
}

input_checker_obj['init'](process.argv)
if (!input_checker_obj['is_valid']){
  process.exit(0)
}

stop_words_obj['init'](process.argv)
data_storage_obj['init'](process.argv)

for (word of data_storage_obj['words']()){
    if (!stop_words_obj['is_stop_word'](word)){
      word_freqs_obj['increment_count'](word)
    }
}

[freqs, freq] = word_freqs_obj['freqs']()

for (var i = 0; i < 25; i++){
  while (!(freq in freqs) || (freqs[freq].size == 0)) freq --
  word = freqs[freq].values().next().value
  console.log(`${word} - ${freq}`)
  freqs[freq].delete(word)
}