# FLIGHT INDOOR
## Pha thu thập dữ liệu và Huấn luyện mô hình
Pha thu thập dữ liệu sẽ được miêu tả chi tiết trong file word Chuẩn bị dữ liệu và huấn luyện mô hình tránh vật cản.docx.
Quy trình thu thập dữ liệu được chia thành 3 giai đoạn
### Giai đoạn 1: thu thập dữ liệu thực tế.
#### Mô tả: trong giai đoạn 1 ta cần hiểu rõ việc chia các luồng di chuyển được quy định về an toàn. Trong bài báo sẽ chia hành lang C7 ra thành 11 luồng di chuyển với 6 luồng là nguy hiểm và 5 luồng là luồng an toàn phù hợp cho việc đi thẳng
##### Trường hợp có cửa
Trong đó các luồng an toàn cho phép DJI có thể di chuyển tịnh tiến với 1 tốc độ đã được quy định quá trình lấy dữ liệu 5 hàng tính từ hàng giữa cách đều về 2 phía 30(cm) là khu vực di chuyển an toàn
6 luồng nguy hiểm bên trái và phải được chia thành cách khoảng cách tường 40 60 80(cm).
Với mỗi luồng ta lấy dữ liệu cả chiều đi lẫn chiều về
##### Trường hợp không có cửa
Ta lấy dữ liệu của các hàng tính từ giữa ra cách 30(cm) cũng làm 2 lần đi và về.
### Giai đoạn 2: xử lý dữ liệu
#### Đầu tiên, ta sẽ upload dữ liệu lên trên link drive và kết nối với Google Colab với drive để giúp việc capture ảnh và phân loại ảnh để train.
Ta upload lên trên drive cùng với việc sẽ đặt tên cho các đoạn video theo tên của các lần lấy dữ liệu tương ứng cùng các trường hợp lấy dữ liệu có khá nhiều trường hợp cần lấy dữ liệu dựa trên file DOCX.
Sau khi remane và up lên drive ta sẽ chạy đoạn code capture_resize.ipynb để có thể capture các frame ảnh cũng như đưa ảnh về 1 kích cỡ nhất định.
#####  Chú ý phải chỉnh sửa các đường dẫn đến đúng file.
#### Bước 2: ta cần phải chia dữ liệu ra thành các file đúng với loại ảnh mà ta đã quy định vì bài toán dựa trên việc sẽ sử dụng bài toán classification 
Khi đã xong giai đoạn cắt ảnh cùng với resize ta sẽ sử dụng FLIGHT INDOOR
## Pha thu thập dữ liệu và Huấn luyện mô hình
Pha thu thập dữ liệu sẽ được miêu tả chi tiết trong file word Chuẩn bị dữ liệu và huấn luyện mô hình tránh vật cản.docx.
Quy trình thu thập dữ liệu được chia thành 3 giai đoạn
### Giai đoạn 1: thu thập dữ liệu thực tế.
#### Mô tả: trong giai đoạn 1 ta cần hiểu rõ việc chia các luồng di chuyển được quy định về an toàn. Trong bài báo sẽ chia hành lang C7 ra thành 11 luồng di chuyển với 6 luồng là nguy hiểm và 5 luồng là luồng an toàn phù hợp cho việc đi thẳng
##### Trường hợp có cửa
Trong đó các luồng an toàn cho phép DJI có thể di chuyển tịnh tiến với 1 tốc độ đã được quy định quá trình lấy dữ liệu 5 hàng tính từ hàng giữa cách đều về 2 phía 30(cm) là khu vực di chuyển an toàn
6 luồng nguy hiểm bên trái và phải được chia thành cách khoảng cách tường 40 60 80(cm).
Với mỗi luồng ta lấy dữ liệu cả chiều đi lẫn chiều về
##### Trường hợp không có cửa
Ta lấy dữ liệu của các hàng tính từ giữa ra cách 30(cm) cũng làm 2 lần đi và về.
### Giai đoạn 2: xử lý dữ liệu
#### Đầu tiên, ta sẽ upload dữ liệu lên trên link drive và kết nối với Google Colab với drive để giúp việc capture ảnh và phân loại ảnh để train.
Ta upload lên trên drive cùng với việc sẽ đặt tên cho các đoạn video theo tên của các lần lấy dữ liệu tương ứng cùng các trường hợp lấy dữ liệu có khá nhiều trường hợp cần lấy dữ liệu dựa trên file DOCX.
Sau khi remane và up lên drive ta sẽ chạy đoạn code capture_resize.ipynb để có thể capture các frame ảnh cũng như đưa ảnh về 1 kích cỡ nhất định.
#####  Chú ý phải chỉnh sửa các đường dẫn đến đúng file.
#### Bước 2: ta cần phải chia dữ liệu ra thành các file đúng với loại ảnh mà ta đã quy định vì bài toán dựa trên việc sẽ sử dụng bài toán classification 
Khi đã xong giai đoạn cắt ảnh cùng với resize ta chia thành các file với các trường hợp cố định là RIGHT,LEFT,LOITER,FORWARD.
Ta sử dụng file code divide_data.ipynb
#### Bước 3: ta sẽ huấn luyện mô hình với dữ liệu đã thu thập được
Ta sẽ sử dụng file code sau TrainModel.ipynb.
##### Chú ý ta cần phải sửa đúng các đường link đến các folder trong file code cùng file drive để có thể huấn luyện được mô hình.

