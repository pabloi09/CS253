//CLOSED MAPS
//Didn't realized 15.2 was there. Implementing it here
const fs = require('fs')

class WordFrequencyFramework {
  _load_event_handlers = []
  _dowork_event_handlers = []
  _end_event_handlers = []

  register_for_load_event(handler){
    this._load_event_handlers.push(handler)
  }
  register_for_dowork_event(handler){
    this._dowork_event_handlers.push(handler)
  }
  register_for_end_event(handler){
    this._end_event_handlers.push(handler)
  }
  run(args){
    this._load_event_handlers.forEach(handler => handler(args))
    this._dowork_event_handlers.forEach(handler => handler())
    this._end_event_handlers.forEach(handler => handler())
  }
}

class StopWordFilter {
  
  constructor(wff){
    this._stop_words = []
    wff.register_for_load_event(this._load.bind(this))
  }

  _load(args){
    this._stop_words = new Set(fs.readFileSync(args[2], 'utf-8').split(','))
  }

  is_stop_word(word){
    return this._stop_words.has(word)
  }
}


class DataStorage {
  _data = []
  _stop_word_filter = null
  _word_event_handlers = []

  constructor(wff, stop_word_filter){
    this._stop_word_filter = stop_word_filter
    wff.register_for_load_event(this._load.bind(this))
    wff.register_for_dowork_event(this._produce_words.bind(this))
  }

  _load(args){
    this._data = fs.readFileSync(args[3], 'utf-8').replace(/[\W_]+/g," ").toLowerCase().split(" ")
  }

  _produce_words(){
    for (var word of this._data){
      if (word.length > 1 && !this._stop_word_filter.is_stop_word(word)){
        for (var handler of this._word_event_handlers){
          handler(word)
        }
      }
    }
  }

  register_for_word_event(handler){
    this._word_event_handlers.push(handler)
  }
  
}

class WordFrequencyCounter{
  _wordCount = {}
  _freqs = {}
  _max_freq = 0

  constructor(wff, data_storage){
    data_storage.register_for_word_event(this._increment_count.bind(this))
    wff.register_for_dowork_event(this._get_freqs.bind(this))
    wff.register_for_end_event(this._print_freqs.bind(this))
  }

  _increment_count(word){
    this._wordCount[word] = this._wordCount[word] ? this._wordCount[word]+1 : 1
  }

  _get_freqs(){
    for (var word in this._wordCount){
      var freq = this._wordCount[word]
      if (!this._freqs[freq]) this._freqs[freq] = new Set()
      this._freqs[freq].add(word)
      this._max_freq = Math.max(this._max_freq, freq)
    }
  }

  _print_freqs(){
    var freq = this._max_freq
    for (var i = 0; i < 25; i++){
      while (!(freq in this._freqs) || (this._freqs[freq].size == 0)) freq --
      var word = this._freqs[freq].values().next().value
      console.log(`${word} - ${freq}`)
      this._freqs[freq].delete(word)
    }
  }
}

class WordZCounter{
  _zCounter = 0

  constructor(wff, data_storage){
    data_storage.register_for_word_event(this._increment_count.bind(this))
    wff.register_for_end_event(this._print_z_counter.bind(this))
  }

  _increment_count(word){
    if (word.includes("z")) this._zCounter +=1
  }


  _print_z_counter(){
      console.log(`Words with z - ${this._zCounter}`)
  }
}

wff = new WordFrequencyFramework()
stop_word_filter = new StopWordFilter(wff)
data_storage = new DataStorage(wff, stop_word_filter)
word_freq_counter = new WordFrequencyCounter(wff, data_storage)
word_z_counter = new WordZCounter(wff, data_storage)
wff.run(process.argv)

