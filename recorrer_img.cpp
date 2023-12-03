#inc lude <ios t ream>
#inc lude <opencv2 / opencv . hpp>
us ing namespace cv ;
us ing namespace s td ;
// read the image f i l e .
Mat image = imread (” ima g en a c o l o r . jpg ” , IMREAD COLOR) ;
// read each p i x e l (RGB p i x e l ) .
	for (int r=0; r<image . rows ; r++) {
		for (int c=0; c<image . c o l s ; c++) {
			Point3 <uchar>∗ p = image . ptr<Point3 <uchar> >(r , c ) ;
// x :B y :G z :R
			printf (”(%d %d %d) ” , p−>x , p−>y , p−>z ) ;
	}
}