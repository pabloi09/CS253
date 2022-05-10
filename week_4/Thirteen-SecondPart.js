//Continuation-passing style
//Didn't realized 13.2 and 13.3 were there. Implementing them here
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
//To implement a context (self) we can use a strategy Python uses. It is: passing the prototype itself as a default argument of every method inside the prototype. The language could do it automatically so we don't have to write it as an argument of the function
input_checker_obj = {
  'data': true,
  'init': (self, args) =>  checkInput(args, self),
  'is_valid': (self) => self['data']
}

stop_words_obj = {
  'data' : [],
  'init' : (self, args) => readStopWords(args[2], self),
  'is_stop_word': (self, word) => (word.length <= 1 || self['data'].has(word))
}

data_storage_obj = {
  'data' : [],
  'init': (self, args) => readInputFile(args[3], self),
  'words': (self) => self['data']
}

word_freqs_obj = {
  'data' : {},
  'increment_count': (self, word) => self['data'][word] = self['data'][word] ? self['data'][word]+1 : 1,
  'freqs': (self) => getFrequencies(self['data'])
}

input_checker_obj['init'](input_checker_obj, process.argv)
if (!input_checker_obj['is_valid']){
  process.exit(0)
}

stop_words_obj['init'](stop_words_obj, process.argv)
data_storage_obj['init'](data_storage_obj, process.argv)

for (word of data_storage_obj['words'](data_storage_obj)){
    if (!stop_words_obj['is_stop_word'](stop_words_obj, word)){
      word_freqs_obj['increment_count'](word_freqs_obj, word)
    }
}

[freqs, freq] = word_freqs_obj['freqs'](word_freqs_obj)

word_freqs_obj['top25'] = (freqs, freq) => {
  for (var i = 0; i < 25; i++){
    while (!(freq in freqs) || (freqs[freq].size == 0)) freq --
    word = freqs[freq].values().next().value
    console.log(`${word} - ${freq}`)
    freqs[freq].delete(word)
 }
}

word_freqs_obj['top25'](freqs, freq)


