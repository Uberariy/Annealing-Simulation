"""
This program is an .ipybn notebook utility, used to convert special data into excel.

"""

# %%
'''Imports'''
import sys
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import pandas as pd
import math
import os
from collections import defaultdict

# %%
'''Functions cell'''
def dict_to_df(d, progons):
    s = pd.Series(d, index=pd.MultiIndex.from_tuples(d))
    df = s.unstack()
    return df.iloc[1:, 1:] / progons

def draw_time_heatmap(path):
    with open(path, 'r') as f:
        text = f.read()
    d = defaultdict(float)
    for line in text.split("\n"):
        procN = 0
        workN = 0
        time = 0
        for elem in line.split(","):
            if not elem:
                break
            name = elem.split(":")[0]
            content = elem.split(":")[1]
            if name == "Processors":
                procN = int(content)
            if name == "Works":
                workN = int(content)
            if name == "Duration":
                time = float(content)
        d[(procN, workN)] += time
    plt.figure(figsize=(15, 15))
    sns.heatmap(dict_to_df(d, 5), annot=True)
    plt.title('Time of algorithm execution')
    plt.xlabel('Processors Number')
    plt.ylabel('Works Number')

def draw_temperature_heatmap(path):
    with open(path, 'r') as f:
        text = f.read()
    d = defaultdict(float)
    d2 = defaultdict(float)
    for line in text.split("\n"):
        procN = 0
        workN = 0
        time = 0
        law = ""
        start_res = 0
        final_res = 1
        for elem in line.split(","):
            if not elem:
                break
            name = elem.split(":")[0]
            content = elem.split(":")[1]
            if name == "Processors":
                procN = int(content)
            if name == "Works":
                workN = int(content)
            if name == "Duration":
                time = float(content)
            if name == "Law":
                if int(content) == 0:
                    law = "Boltzman Law"
                elif int(content) == 1:
                    law = "Cauchy Law"
                elif int(content) == 2:
                    law = "Natural Log Law"
            if name == "StartCriterion":
                start_res = int(content)
            if name == "FinalCriterion":
                final_res = int(content)
        d[((procN, workN), law)] += time
        d2[((procN, workN), law)] += start_res / final_res
    print(dict_to_df(d2, 5))
    plt.figure(figsize=(6, 6))
    anno = dict_to_df(d2, 5)
    print(anno)
    sns.heatmap(dict_to_df(d, 5), annot=anno, fmt=".2%")
    plt.title('Time period and quality (color presents time / percentage is quality upgrade)')
    plt.xlabel('Law')
    plt.ylabel('(Processors, Works) Number')

def draw_parallel(path):
    with open(path, 'r') as f:
        text = f.read()
    d = defaultdict(float)
    d2 = defaultdict(float)
    for line in text.split("\n"):
        procN = 0
        workN = 0
        time = 0
        law = ""
        start_res = 0
        final_res = 1
        for elem in line.split(","):
            if not elem:
                break
            name = elem.split(":")[0]
            content = elem.split(":")[1]
            if name == "Threads":
                thread = int(content)
            if name == "Processors":
                procN = int(content)
            if name == "Works":
                workN = int(content)
            if name == "Duration":
                time = float(content)
            if name == "Law":
                if int(content) == 0:
                    law = "Boltzman Law"
                elif int(content) == 1:
                    law = "Cauchy Law"
                elif int(content) == 2:
                    law = "Natural Log Law"
            if name == "StartCriterion":
                start_res = int(content)
            if name == "FinalCriterion":
                final_res = int(content)
        d[thread] += time
        d2[thread] += start_res / final_res
    plt.figure(figsize=(10, 6))
    plt.plot(sorted(d.keys()), [d[i] / 5 for i in sorted(d.keys())], label='Predicted Mean Speed (Kbit/s)', color='#0DD26A')
    plt.title('Mean annealing algorithm execution time')
    plt.xlabel('Number of threads')
    plt.ylabel('Execution time')
    plt.figure(figsize=(10, 6))
    plt.plot(sorted(d.keys()), [d2[i] / 5 for i in sorted(d.keys())], label='Predicted Mean Speed (Kbit/s)', color='#DE630B')
    plt.title('Mean quality upgrade fraction')
    plt.xlabel('Number of threads')
    plt.ylabel('Loss criterion minimisation fraction')


# %%
if __name__ == "__main__":
    draw_time_heatmap("consequitivelog1")
    draw_temperature_heatmap("consequitivelog2")

# %%

if __name__ == "__main__":
    draw_parallel("parallellog3")

# %%
