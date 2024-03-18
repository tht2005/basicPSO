# Code giải gần đúng bài toán TSP (trường hợp đồ thị đầy đủ).

## main.py
    Input: Số đỉnh của đồ thị và số bộ kiểm thử
    => Tạo ra các bộ kiểm thử, và tính toán thời gian chạy và mức độ chính xác của thuật toán gần đúng.

    Chạy chương trình:
        # Tại thư mục gốc của project
        # Do chương trình biên dịch 2 code cpp trước khi chạy, sử dụng địa chỉ tương đối
        $ python3 ./src/main.py
    

## tsp_pso.cpp
    Code thuật toán gần đúng giải bài toán TSP

    Biên dịch và chạy:
        # Cho input là n và ma trận n x n trong file a.inp
        # In ra độ dài đường đi và cách đi tìm được
        $ g++ tsp_pso.cpp -o tsp_pso
        $ ./tsp_pso a.inp a.out

## tsp_n_small.cpp
    Code thuật toán chính xác giải bài toán TSP với số đỉnh bé (n <= 22)
    Sử dụng thuật toán quy hoạch động.

    Biên dịch và chạy tương tự tsp_pso.cpp

