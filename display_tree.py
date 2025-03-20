import tkinter as tk
from pathlib import Path

CW = 1000
CH = 600
SCALE = 15
SEED_RADIUS = 5

tree_it = 0


def str_to_pos(string):
    x, y = string.split(' ')
    return 300 + float(x) * SCALE, CH - 40 - float(y) * SCALE  # Tkinter is dumb


def draw(fp):
    w.delete("all")
    with open(fp) as file:
        contents = file.readlines()
    for line in contents:
        if line[0] == "s":
            p1, p2 = line.split(',')
            p1, p2 = str_to_pos(p1[1:]), str_to_pos(p2)
            w.create_line(*p1, *p2)
        elif line[0] == "*":
            x, y = str_to_pos(line[1:])
            w.create_oval(x - SEED_RADIUS, y - SEED_RADIUS, x + SEED_RADIUS, y + SEED_RADIUS, fill="black")


def left_but():
    global tree_it
    tree_it -= 1
    if tree_it < 0:
        tree_it = len(trees) - 1

    draw(trees[tree_it])


def right_but():
    global tree_it
    tree_it += 1
    if tree_it >= len(trees):
        tree_it = 0

    draw(trees[tree_it])


out = "./cmake-build-release/out/"
trees = [out + "fittest_tree.tree"]
for p in (Path(out) / "forest").iterdir():
    trees.append(p)

master = tk.Tk()
master.geometry(str(CW) + 'x' + str(CH))
lb = tk.Button(master, width=20, text="<", command=left_but)
rb = tk.Button(master, width=20, text=">", command=right_but)
w = tk.Canvas(master, width=600)
lb.pack(side=tk.LEFT, expand=True, fill=tk.Y)
w.pack(side=tk.LEFT, expand=True, fill=tk.BOTH)
rb.pack(side=tk.LEFT, expand=True, fill=tk.Y)
draw(trees[0])

tk.mainloop()