--CREATE for tabls shoppingcart and product by Chao Liu / code row 60 to 106
CREATE TABLE shoppingcart(
    cartId INT PRIMARY KEY,
    cartName VARCHAR(30) NOT NULL,
    cartSize INT NOT NULL,
    cartDate DATE NOT NULL, 
    cartTotalprice NUMBER(10,2),
    cartNumberofproducts INT NOT NULL
);

CREATE TABLE product(
    productId INT PRIMARY KEY,
    productName VARCHAR(50) NOT NULL,
    productPrice NUMBER(10,2) NOT NULL,
    productDesciption VARCHAR(100) NOT NULL,
    productStatus VARCHAR(30) NOT NULL,
    productInCart INT NOT NULL,
    CONSTRAINT productInCart_fk FOREIGN KEY (productInCart) REFERENCES shoppingcart(cartId)
);
DESCRIBE shoppingcart;
DESCRIBE products;
-- insert to table shoppingcart --10 row total
INSERT ALL 
INTO shoppingcart VALUES(1, 'Cart_01', 100, to_date('2021-02-17','yyyy-mm-dd'), 1200.34, 10)
INTO shoppingcart VALUES(2, 'Cart_02', 100, to_date('2021-02-16','yyyy-mm-dd'), 340.56, 5)
INTO shoppingcart VALUES(3, 'Cart_03', 100, to_date('2021-02-15','yyyy-mm-dd'), 570.21, 8)
INTO shoppingcart VALUES(4, 'Cart_04', 100, to_date('2021-02-14','yyyy-mm-dd'), 1130.47, 12)
INTO shoppingcart VALUES(5, 'Cart_05', 100, to_date('2021-02-13','yyyy-mm-dd'), 330.35, 4)
INTO shoppingcart VALUES(6, 'Cart_06', 100, to_date('2021-03-01','yyyy-mm-dd'), 2210.99, 20)
INTO shoppingcart VALUES(7, 'Cart_07', 100, to_date('2021-03-05','yyyy-mm-dd'), 1550.68, 15)
INTO shoppingcart VALUES(8, 'Cart_08', 100, to_date('2021-03-10','yyyy-mm-dd'), 465.51, 7)
INTO shoppingcart VALUES(9, 'Cart_09', 100, to_date('2021-03-11','yyyy-mm-dd'), 880.87, 9)
INTO shoppingcart VALUES(10, 'Cart_10', 100, to_date('2021-03-20','yyyy-mm-dd'), 659.12, 6)
SELECT * FROM DUAL;

-- insert to table product --10 row total
INSERT ALL
INTO product VALUES(20031, 'Router', 14.99, 'WIFI Router', 'IN STOCK', 1)
INTO product VALUES(20421, 'Multi Charging Cable', 19.99, 'Multiple USB Fast Charging', 'IN STOCK', 2)
INTO product VALUES(20397, 'WiFi Smart Light Bulb', 31.99, 'Remote Contorl light bulb', 'IN STOCK', 3)
INTO product VALUES(20541, 'Wireless Headphones', 30.99, 'Wireless On-Ear Headphones', 'IN STOCK', 4)
INTO product VALUES(20212, 'Bread maker', 17.99, 'New Bread maker', 'IN STOCK', 5)
INTO product VALUES(20997, 'Calculator', 8.99, 'Calculator regular size', 'IN STOCK', 6)
INTO product VALUES(20859, 'Clock', 21.99, 'Digital time clock', 'IN STOCK', 7)
INTO product VALUES(20671, 'Coffee maker', 40.99, 'Multi function Coffee maker', 'IN STOCK', 8)
INTO product VALUES(20754, 'IPhone 14', 1699.99, 'IPhone 14 series', 'Discontinued', 9)
INTO product VALUES(20511, 'MacBook Master', 3100.99, 'MacBook Master series', 'Discontinued', 10)
SELECT * FROM DUAL;

--CREATE for tables purchase and payment by Chirag Mudgil / code row 110 to 168 -- fixed by Chao Liu

CREATE TABLE purchase(
    id INT PRIMARY KEY,
    firstName VARCHAR(20) NOT NULL,
    lastName VARCHAR(20) NOT NULL,
    product_quantity VARCHAR(30) NOT NULL UNIQUE, 
    payment_gateway VARCHAR(10),
    summary VARCHAR(20)
);

CREATE TABLE payment(
    id INT PRIMARY KEY,
    gateway VARCHAR(30) NOT NULL,
    login VARCHAR(30) NOT NULL UNIQUE,
    password VARCHAR(30) NOT NULL UNIQUE,
    purchaseAmount VARCHAR(20) NOT NULL,
    amount VARCHAR(20) NOT NULL,
    summary VARCHAR(20) NOT NULL,
    purchaseNumber INT NOT NULL,
    link_ID INT NOT NULL,
    CONSTRAINT link_ID_fk FOREIGN KEY (link_ID) REFERENCES purchase(ID)
);

DESCRIBE purchase;
DESCRIBE payment;
-- insert to table purchase --10 row total

INSERT ALL 
INTO purchase VALUES(1, 'Sam', 'Wicky', '18', 'paypal', 'success')
INTO purchase VALUES(2, 'John', 'Wick', '21', 'paypal', 'success')
INTO purchase VALUES(3, 'Betty', 'Mac', '16', 'paytm', 'success')
INTO purchase VALUES(4, 'Amy', 'Williams', '26', 'googlepay', 'success')
INTO purchase VALUES(5, 'Mary', 'Jane', '19', 'phonepay', 'success')
INTO purchase VALUES(6, 'Antonio', 'Garza', '20', 'paypal', 'success')
INTO purchase VALUES(7, 'Batman', 'SuperMan', '24', 'paytm', 'success')
INTO purchase VALUES(8, 'Romam', 'Burt', '23', 'googlepay', 'success')
INTO purchase VALUES(9, 'Tom', 'Hanks', '17', 'googlepay', 'success')
INTO purchase VALUES(10, 'Essa', 'Hussain', '25', 'paypal', 'success')
SELECT * FROM DUAL;

-- insert to table payment --14 row total

INSERT ALL
INTO payment VALUES(1,'paypal', 'samWickHandSome', 'e4Y2ElVANX', '500', '500','success', 123,1)
INTO payment VALUES(2,'paypal', 'samWickSmallAc', 'd452KK!lVps', '100', '100','success', 124,1)
INTO payment VALUES(3,'paypal', 'ikeajohnwick', 'lRPqbfrB3Hpr', '320', '320','success', 125,2)
INTO payment VALUES(4,'paytm', 'bettymactow', 'nQOTz9eBrMXk', '460', '460','success', 126,3)
INTO payment VALUES(5,'googlepay', 'bettymactow2', 'Goodpw123', '250', '250','success', 127,3)
INTO payment VALUES(6,'googlepay', 'amywillsea', 'amyDumbPW!', '563', '563','success', 222,4)
INTO payment VALUES(7,'phonepay', 'maryjanemj', 'maryJanePretty', '569', '569','success', 234,5)
INTO payment VALUES(8,'phonepay', 'maryisRich', 'maryJaneRich', '155', '155','success', 523,5)
INTO payment VALUES(9,'paypal', 'iamAntion', '321poorAnton321', '551', '551','success', 136,6)
INTO payment VALUES(10,'paytm', 'batSupBeatsUp', 'Batman01Sup', '275', '275','success', 174,7)
INTO payment VALUES(11,'paytm', 'batSupAc2', 'BatSup222', '321', '321','success', 134,7)
INTO payment VALUES(12,'googlepay', 'Romanishere', 'rowRom0297', '399', '399','success', 138,8)
INTO payment VALUES(13,'googlepay', 'IAMACTOR', '288TOM288HANKS', '321', '321','success', 129,9)
INTO payment VALUES(14,'paypal', 'EssaReturn', 'wasB9!w@', '412', '412','success', 110,10)
SELECT * FROM DUAL;

COMMIT;