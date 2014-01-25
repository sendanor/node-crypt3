{
  "targets": [
    {
      "target_name": "crypt3",
      "sources": [ "crypt3.cc" ],
      "conditions": [
        ['OS!="mac"', {
          'link_settings': { "libraries": [ "-lcrypt" ] }
        }]
      ]
    }
  ]
}
