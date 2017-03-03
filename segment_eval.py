from sys import argv, stderr

if len(argv) != 3:
    stderr.write("%s sys_file gold_file", (argv[0]))
    exit(1)

sys_data = [l.split(b'\t') for l in open(argv[1],'rb').read().split(b'\n') if l != b'']
gold_data = [l.split(b'\t') for l in open(argv[2],'rb').read().split(b'\n') if l != b'']

sys_s_found = 0
sys_s_tot = 0
gold_s_found = 0
gold_s_tot = 0

sys_um_found = 0
sys_um_tot = 0
gold_um_found = 0
gold_um_tot = 0

for i, gold_line in enumerate(gold_data):    
    sys_line = sys_data[i]
    
    gold_wf, gold_segments = gold_line[0], gold_line[1].split(b' ')
    sys_wf, sys_segments = sys_line[0], sys_line[1].split(b' ')

    gold_word = b''.join(gold_segments)
    sys_word = b''.join(sys_segments)

    if not (gold_wf == sys_wf == gold_word == sys_word):
        print(gold_wf)
        print(sys_wf)
        print(gold_word)
        print(sys_word)
        assert(0)


    u_gold_morpheme_dict = set()
    u_sys_morpheme_dict = set()
    gold_splits = set()
    sys_splits = set()
    acc = 0

    for substr in gold_segments:
        u_gold_morpheme_dict.add((acc,acc + len(substr)))
        if acc + len(substr) < len(gold_word):
            gold_splits.add(acc + len(substr))
        acc += len(substr)

    acc = 0 

    for substr in sys_segments:
        u_sys_morpheme_dict.add((acc,acc + len(substr)))
        if acc + len(substr) < len(sys_word):
            sys_splits.add(acc + len(substr))
        acc += len(substr)

    for morpheme in u_sys_morpheme_dict:
        if morpheme in u_gold_morpheme_dict:
            sys_um_found += 1
        sys_um_tot += 1

    for morpheme in u_gold_morpheme_dict:
        if morpheme in u_sys_morpheme_dict:
            gold_um_found += 1
        gold_um_tot += 1

    for s in sys_splits:
        if s in gold_splits:
            sys_s_found += 1
        sys_s_tot += 1

    for s in gold_splits:
        if s in sys_splits:
            gold_s_found += 1
        gold_s_tot += 1

recall = gold_um_found * 1.0 / gold_um_tot
precision = sys_um_found * 1.0 / sys_um_tot

print ("Unlabeled morpheme recall: %.3f" % recall)
print ("Unlabeled morpheme precision: %.3f" % precision)
print ("Unlabeled morpheme f-score : %.3f" % (2 * precision * recall / (precision + recall)))
print()

recall = gold_s_found * 1.0 / gold_s_tot
precision = sys_s_found * 1.0 / sys_s_tot

print ("Boundary recall: %.3f" % recall)
print ("Boundary precision: %.3f" % precision)
print ("Boundary f-score : %.3f" % (2 * precision * recall / (precision + recall)))
