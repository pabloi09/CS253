//Continuation-passing style

const fs = require('fs')

function checkInput(args, func){
  if (args.length != 4){
    console.log("Please, as an input, provide:\n1. The stop words file \n2.The file to analyse")
    process.exit(0)
  }
  func(args, readInputFile)
}

function readStopWords(args, func){
  stopWords = new Set(fs.readFileSync(args[2], 'utf-8').split(','))
  func(args, stopWords, filterCount)
}

function readInputFile(args, stopWords, func){
  words = fs.readFileSync(args[3], 'utf-8').replace(/[\W_]+/g," ").toLowerCase().split(" ")
  func(stopWords, words, getFrequencies)
}

function filterCount(stopWords, words, func){
  wordCount = {}
  for (word of words){
    if (word.length > 1 && !stopWords.has(word)){
      if (word in wordCount) { wordCount[word] += 1}
      else { wordCount[word] = 1}
    }
  }
  func(wordCount, print25MostFrequent)
}

function getFrequencies(wordCount, func){
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
  func(freqs, maxFreq, no_op)
}

function print25MostFrequent(freqs, maxFreq,func){
  freq = maxFreq
  for (var i = 0; i < 25; i++){
    while (!(freq in freqs) || (freqs[freq].size == 0)) freq --
    word = freqs[freq].values().next().value
    console.log(`${word} - ${freq}`)
    freqs[freq].delete(word)
  }
  func(null)
  
}

function no_op(func){
  return
}

checkInput(process.argv, readStopWords)