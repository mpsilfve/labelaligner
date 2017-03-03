from sys import argv, stderr

if len(argv) != 3:
    stderr.write("%s sys_file gold_file", (argv[0]))
    exit(1)

sys_data = [l.split(b' ') for l in open(argv[1],'rb').read().split(b'\n') if l != b'']
gold_data = [l.split(b' ') for l in open(argv[2],'rb').read().split(b'\n') if l != b'']

sys_s_found = 0
sys_s_tot = 0
gold_s_found = 0
gold_s_tot = 0

sys_um_found = 0
sys_um_tot = 0
gold_um_found = 0
gold_um_tot = 0
sys_lm_found = 0
sys_lm_tot = 0
gold_lm_found = 0
gold_lm_tot = 0

for i, gold_line in enumerate(gold_data):
    sys_line = sys_data[i]
    
    gold_line = [x.split(b'/') for x in gold_line]
    sys_line = [x.split(b'/') for x in sys_line]

    gold_word = b''.join([x[0] for x in gold_line if x[0] != b'""'])
    sys_word = b''.join([x[0] for x in sys_line if x[0] != b'""'])    
    assert(gold_word == sys_word)

    u_gold_morpheme_dict = set()
    l_gold_morpheme_dict = set()
    gold_splits = set()
    acc = 0

    for substr, labels in gold_line:
        labels = labels.split(b',')
        u_gold_morpheme_dict.add((acc,acc + len(substr)))
        for l in labels:
            l_gold_morpheme_dict.add((acc,acc + len(substr),l))
        if acc + len(substr) < len(gold_word):
            gold_splits.add(acc + len(substr))
        acc += len(substr)

    u_sys_morpheme_dict = set()
    l_sys_morpheme_dict = set()
    sys_splits = set()
    acc = 0
    for substr, labels in sys_line:
        labels = labels.split(b',')
        u_sys_morpheme_dict.add((acc,acc + len(substr)))
        for l in labels:
            l_sys_morpheme_dict.add((acc,acc + len(substr),l))
        if acc + len(substr) < len(gold_word):
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

    for morpheme in l_sys_morpheme_dict:
        if morpheme in l_gold_morpheme_dict:
            sys_lm_found += 1
        sys_lm_tot += 1

    for morpheme in l_gold_morpheme_dict:
        if morpheme in l_sys_morpheme_dict:
            gold_lm_found += 1
        gold_lm_tot += 1

    for s in sys_splits:
        if s in gold_splits:
            sys_s_found += 1
        sys_s_tot += 1

    for s in gold_splits:
        if s in sys_splits:
            gold_s_found += 1
        gold_s_tot += 1

recall = gold_lm_found * 1.0 / gold_lm_tot
precision = sys_lm_found * 1.0 / sys_lm_tot

print ("Labeled morpheme recall: %.3f" % recall)
print ("Labeled morpheme precision: %.3f" % precision)
print ("Labeled morpheme f-score : %.3f" % (2 * precision * recall / (precision + recall)))
print()

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
