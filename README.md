# Ceaser Algorithm

## Description
Generates a String of Characters and Takes The String and Adds a Displacement  
to Each Character Shifting it by a Certain Number. Once the String has been  
Shifted, the Program Shifts it Back to Ensure no Corruption in the Algorithm.

## Files
| File               | Language | Library | Description                                              |
|--------------------|:--------:|---------|----------------------------------------------------------|
| Ceasar_CUDA        | C        | CUDA    | Processes information on GPU threads                     |
| Ceasar_MPI_Scatter | C        | MPI     | Processes information on CPU processes (Unshared Memory) |
| Ceasar_MPI_Send    | C        | MPI     | Processes information on CPU processes (Unshared Memory) |
| Ceasar_OMP         | C        | OpenMP  | Processes information on CPU threads   (shared Memory)   |
