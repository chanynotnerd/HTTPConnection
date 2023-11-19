# HTTP Connection
---
#### 실행결과

![개발자도구 내 200 응답 확인](https://github.com/chanynotnerd/HTTPConnection/assets/63177308/32dd7d29-fc6b-4c20-87c5-c78988ae8619)
****
#### C++를 사용하여 TCP 서버 구현, HTML 파일 랜더링

> 1. 클라이언트가 서버에 접속 / 서버의 IP, Port 주소 사용
> 2. 서버-클라이언트 연결
> 3. 클라이언트가 URL 검색(서버와 연결 확인 후 URL 검색)
> 4. 클라이언트가 URL을 서버에 요청(HTTP 프로토콜 사용하여 서버로 요청을 전송).
> 5. 서버가 클라이언트 요청 수행(URL에 해당하는 리소스 검색)
> 6. 서버가 클라이언트에게 요청에 대한 응답(웹 페이지)을 보내줌.
> 7. 클라이언트의 웹 브라우저에 서버로부터 받은 응답(웹페이지 렌더링) 표시
***
#### QA

> 질문점: 클라이언트 종료 후 웹페이지가 뜬다.<br/>
> 원인: HTTP 프로토콜의 특성 중 하나인 "stateless" 특징 때문.<br/> 
> 해결법: 연결종료시키지 않고 서버와 통신하는 방법으로 구성해도 됨.
***
 #### 구현 방법 따른 장단점
 
|구분|클라 종료 x, 서버 통신 유지|클라 종료 후 웹페이지 표시|
|------|------|------|
|장점|실시간 업데이트 가능|서버 리소스 효율 증대|
|단점|네트워크 대역폭 상시 사용|실시간 업데이트 불가|
