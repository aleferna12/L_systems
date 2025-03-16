import tkinter as tk

CW = 600
CH = CW
SCALE = 15
SEED_RADIUS = 5


def str_to_pos(string):
    x, y = string.split(' ')
    return CW / 2 + float(x) * SCALE, CH - float(y) * SCALE  # Tkinter is dumb


fp = "./cmake-build-release/out/fittest_tree.tree"
with open(fp) as file:
    contents = file.readlines()

master = tk.Tk()
w = tk.Canvas(master, width=CW, height=CH)
w.pack()

for line in contents:
    if line[0] == "s":
        p1, p2 = line.split(',')
        p1, p2 = str_to_pos(p1[1:]), str_to_pos(p2)
        w.create_line(*p1, *p2)
    elif line[0] == "*":
        x, y = str_to_pos(line[1:])
        w.create_oval(x - SEED_RADIUS, y - SEED_RADIUS, x + SEED_RADIUS, y + SEED_RADIUS, fill="black")


tk.mainloop()