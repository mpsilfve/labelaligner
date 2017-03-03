from sys import argv, stderr

if len(argv) != 2:
    stderr.write("%s file", (argv[0]))
    exit(1)

data = [l.split(b' ') for l in open(argv[1],'rb').read().split(b'\n') if l != b'']

for i, line in enumerate(data):
    
    line = [x.split(b'/') for x in line]
    word = b''.join([x[0] for x in line if x[0] != b'""'])
    morphemes = b' '.join([x[0] for x in line if x[0] != b'""'])

    print("%s\t%s" % (word,morphemes))
