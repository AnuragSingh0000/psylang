import re
import sys
import csv

def parse_bison_output(file_path):
    with open(file_path, 'r') as f:
        content = f.read()

    state_blocks = re.split(r'\nState (\d+)\n', content)
    
    states = {}
    terminals = set()
    non_terminals = set()
    
    grammar_match = re.search(r'Terminals, with rules where they appear\n\n(.*?)\n\nNonterminals, with rules where they appear', content, re.DOTALL)
    if grammar_match:
        term_text = grammar_match.group(1)
        for line in term_text.split('\n'):
            match = re.match(r'^\s*(\S+)(?:\s+<\S+>)?\s+\(\d+\)', line)
            if match:
                term = match.group(1).strip('"')
                terminals.add(term)

    nt_match = re.search(r'Nonterminals, with rules where they appear\n\n(.*?)\n\n\n', content, re.DOTALL)
    if not nt_match:
         nt_match = re.search(r'Nonterminals, with rules where they appear\n\n(.*?)\n\nState 0', content, re.DOTALL)

    if nt_match:
        nt_text = nt_match.group(1)
        for line in nt_text.split('\n'):
            match = re.match(r'^\s*(\S+)(?:\s+<\S+>)?\s+\(\d+\)', line)
            if match:
                non_terminals.add(match.group(1))

    terminals.add("$end")
    if "error" in terminals: terminals.remove("error")

    for i in range(1, len(state_blocks), 2):
        state_id = int(state_blocks[i])
        block = state_blocks[i+1]
        
        actions = {}
        lines = block.split('\n')
        
        for line in lines:
            line = line.strip()
            if not line: continue
            if '.' in line and not line.startswith('$default'): continue 
            
            match = re.match(r'^(\S+)\s+shift, and go to state (\d+)', line)
            if match:
                token = match.group(1).strip('"')
                target = match.group(2)
                actions[token] = f's{target}'
                continue
            
            match = re.match(r'^(\S+)\s+reduce using rule (\d+)', line)
            if match:
                token = match.group(1).strip('"')
                rule = match.group(2)
                actions[token] = f'r{rule}'
                continue
            
            match = re.match(r'^(\S+)\s+go to state (\d+)', line)
            if match:
                nt = match.group(1)
                target = match.group(2)
                actions[nt] = target
                continue

            if line.startswith('$end') and line.endswith('accept'):
                actions["$end"] = "acc"
                continue

            if line.startswith('$default') and line.endswith('accept'):
                actions['$default'] = 'acc'
                continue

            match = re.match(r'^\$default\s+reduce using rule (\d+)', line)
            if match:
                actions['$default'] = f'r{match.group(1)}'

        states[state_id] = actions

    sorted_terminals = sorted(list(terminals))
    if "$end" in sorted_terminals:
        sorted_terminals.remove("$end")
        sorted_terminals.append("$end")
    
    sorted_non_terminals = sorted(list(non_terminals))
    if "$accept" in sorted_non_terminals: sorted_non_terminals.remove("$accept")
    
    writer = csv.writer(sys.stdout)
    header = ["State"] + sorted_terminals + sorted_non_terminals
    writer.writerow(header)
    
    for state_id in sorted(states.keys()):
        row = [str(state_id)]
        actions = states[state_id]
        default_reduce = actions.get('$default', "")
        
        for t in sorted_terminals:
            val = actions.get(t, "")
            if not val and default_reduce:
                val = default_reduce
            row.append(val)
            
        for nt in sorted_non_terminals:
            row.append(actions.get(nt, ""))
            
        writer.writerow(row)

parse_bison_output('psy.output')
