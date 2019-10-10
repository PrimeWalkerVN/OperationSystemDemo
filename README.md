# Project 1—Simple Shell

Chương trình được thiết kế bằng **ngôn ngữ lập trình C** cho phép người dùng nhập vào các lệnh rổi thực thi chúng trong một tiến trình riêng biệt.
<br> **Source code:** [Click here](https://github.com/PrimeWalkerVN/ProjectOS_1)

## Tổng Quan

#### Chạy một dòng lệnh
Dấu nhắc đầu tiên là `osh>` sau đó người dùng nhập lệnh tiếp theo vào
* Ví dụ `osh>cat prog.c` (cat prog.c là lệnh hiển thị file prog.c trong terminal bằng lệnh `cat` của Unix)
* `osh>exit` để thoát chương trình 
* Nếu dòng lệnh sai chương trình sẽ xuất ra 1 massage `Invalid command`

#### Tiến trình song song
* Khi người dùng nhập 1 lệnh thì chương trình sẽ tạo ra một tiến trình con. Khi tiến trình con kết thúc 
thì tiến trình cha mới tiếp tục được thực hiện
* Vì vậy nếu muốn thực hiện tiến trình con song song với tiến trình cha ta thêm một ký hiệu `&` vào cuối câu lệnh
* Ví dụ: `osh>cat prog.c &` 

## History Feature
* Khi người dùng nhâp ký hiệu `!!` thì chương trình sẽ in ra lệnh trước đó mà người dùng nhập vào. Nếu không có lệnh nào thì chương trình 
sẽ trả về đoạn message sau: `No commands in history`
<br>Ví dụ: `osh>!!`
* *Chi tiết hãy xem trong phần source code*

## Input and Output
#### Input 
* Sử dụng toán tử `<` để nhập thông tin từ  file 
* Ví dụ trong file chứa thông tin là `ls-1` thì khi thực hiện : `osh>ls < input.txt` thì file input sẽ chứa thông tin đóng vai trò giống như lệnh ls
<br> *Khoảng trắng giữa các lệnh như trên là đúng cú pháp, nếu như sai thì lệnh sẽ không thực thi được*

#### Output 
* Sử dụng toán tử `>` để xuất thông tin ra file 
* Ví dụ khi thực hiện : `osh>ls > output.txt` thì file sẽ chứa thông tin mà ls thực hiện, thay vì in ra màn hình thì sẽ in vô file
<br> *Khoảng trắng giữa các lệnh như trên là đúng cú pháp, nếu như sai thì lệnh sẽ không thực thi được*

## Communication via a Pipe 
* Chức năng cho phép dùng output của dòng lệnh này thành input của dòng lệnh khác (tương tự trong phần **Input and Output** ) 
* Ví du: `osh>ls -l | less` thì output của lệnh ls-1 sẽ thành input của lệnh less giống như `osh>ls > output.txt` và `osh>less < output.txt`
* *Khoảng trắng giữa các lệnh như trên là đúng cú pháp, nếu như sai thì lệnh sẽ không thực thi được*
<br> *Chi tiết hãy xem trong phần source code*
