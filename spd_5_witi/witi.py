import itertools
time = 0
MAX=10000

p = 0  #czas trwania
w = 1  #waga
d = 2  #porzadany termin zakonczenia

file_name = open("./test.txt")
content = file_name.read().splitlines()
benchmark = [i.split() for i in content]
tasks = [list(map(int, data)) for data in benchmark]
tasks.pop(0)

number_of_tasks = len(tasks)
for i in range (number_of_tasks):
    time += tasks[i][p]
array_of_tasks = []

for i in range(number_of_tasks):
    current_task = tasks[i]
    tasks.pop(i)
    permutations = list(itertools.permutations(tasks))
    current_best_result = list(permutations[0])
    current_best_result.append(MAX)
    
    for permutation in permutations:
        permutation = list(permutation)
        Ci = 0                            #zaczynamy z zerowym uszeregowaniem
        punishment = 0                    #zerowa kara
        for j in range(number_of_tasks-1):
            Ti = 0
            Ci += permutation[j][p]
            if Ci > permutation[j][d]:
                Ti = Ci - permutation[j][d]
            punishment += permutation[j][w]*Ti
        permutation.append(punishment)
        if punishment  < current_best_result[number_of_tasks-1]:
            current_best_result = permutation

    if time > current_task[d]:
        array_of_tasks.append(current_best_result[number_of_tasks-1] + 
        (current_task[w] * (time - current_task[d])))
    if time <= current_task[d]:
        array_of_tasks.append(current_best_result[number_of_tasks-1])
    tasks.insert(i,current_task)
    
print(current_best_result)
print(min(array_of_tasks))