{
  "targets": [
    {
      "target_name": "crypt3",
      "sources": [ "crypt3.cc" ],
      "include_dirs" : [ "<!(node -e \"require('nan')\")" ],
      "conditions": [
        ['OS!="mac"', {
          'link_settings': { "libraries": [ "-lcrypt" ] }
        }]
      ]
    },
    {
      "target_name": "crypt3async",
      "sources": [ "crypt3async.cc" ],
      "conditions": [
        ['OS!="mac"', {
          'link_settings': { "libraries": [ "-lcrypt" ] }
        }]
      ]
    }
  ]
}
