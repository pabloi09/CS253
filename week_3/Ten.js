//The One

class TFTheOne {
  constructor(value){
    this._value = value
  }
  bind(func){
    this._value = func(this._value)
    return this
  }
  printme(){
    console.log(this._value)
  }
}

const fs = require('fs')

function checkInput(args){
  if (args.length != 4){
    console.log("Please, as an input, provide:\n1. The stop words file \n2.The file to analyse")
    process.exit(0)
  }
  return args
}

function readStopWords(args){
  stopWords = new Set(fs.readFileSync(args[2], 'utf-8').split(','))
  return [args, stopWords]
}

function readInputFile(input){
  [args, stopWords] = input
  words = fs.readFileSync(args[3], 'utf-8').replace(/[\W_]+/g," ").toLowerCase().split(" ")
  return [stopWords, words]
}

function filterCount(input){
  [stopWords, words] = input
  wordCount = {}
  for (word of words){
    if (word.length > 1 && !stopWords.has(word)){
      if (word in wordCount) { wordCount[word] += 1}
      else { wordCount[word] = 1}
    }
  }
  return wordCount
}

function getFrequencies(wordCount){
  freqs = {}
  maxFreq = 0
  for (word in wordCount){
    freq = wordCount[word]
    if (freq in freqs){
      freqs[freq].add(word)
    } else {
      freqs[freq] = new Set([word])
    }
    maxFreq = Math.max(maxFreq, freq)
  }
  return [freqs, maxFreq]
}

function top25(input){
  [freqs, maxFreq] = input
  freq = maxFreq
  top25 = ""
  for (var i = 0; i < 25; i++){
    while (!(freq in freqs) || (freqs[freq].size == 0)) freq --
    word = freqs[freq].values().next().value
    top25 += `${word} - ${freq}\n`
    freqs[freq].delete(word)
  }
  return top25.slice(0,top25.length-1);
}


(new TFTheOne(process.argv)).bind(checkInput)
                            .bind(readStopWords)
                            .bind(readInputFile)
                            .bind(filterCount)
                            .bind(getFrequencies)
                            .bind(top25)
                            .printme()