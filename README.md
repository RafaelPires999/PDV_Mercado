# 🛒 PDV Mercado

Sistema de **Ponto de Venda (PDV)** para mercado, desenvolvido em **C++** e executado no console. Controla vendas, estoque, clientes, fornecedores, usuários e turnos de caixa, com dados salvos em arquivos binários.

## ✨ Funcionalidades

**Acesso e usuários**
- Cadastro do primeiro administrador no primeiro uso do sistema
- Login com dois perfis: **Administrador** e **Caixa**
- Cadastro, listagem, atualização e exclusão de usuários

**Vendas**
- Carrinho de compras com vários produtos por venda
- Verificação de estoque antes de adicionar ao carrinho
- Formas de pagamento: Dinheiro, Cartão (crédito/débito), PIX e Fiado (anotado no cliente)

**Caixa**
- Abertura de turno com valor inicial
- Fechamento de turno com totais por forma de pagamento

**Cadastros**
- Produtos (preço de custo, preço de venda e estoque), com entrada de estoque
- Clientes, com histórico de compras e registro de pagamento de dívidas
- Fornecedores

**Relatórios**
- Histórico de fechamentos de caixa
- Produtos mais vendidos
- Margem de lucro
- Clientes devedores
- Produtos com estoque baixo

## 🛠️ Tecnologias

- C++
- Visual Studio (projeto `.sln` / `.vcxproj`)
- Persistência em arquivos binários (`.dat`) com `FILE*`

## 📁 Arquivos de dados

| Arquivo | Conteúdo |
|---|---|
| `Usuario.dat` | Usuários do sistema |
| `Produtos.dat` | Produtos e estoque |
| `Clientes.dat` | Clientes |
| `Vendas.dat` | Vendas realizadas |
| `Fechamentos.dat` | Fechamentos de caixa |
| `HistCliente.dat` | Histórico de compras dos clientes |

## ▶️ Como executar

1. Clone o repositório:
   ```bash
   git clone https://github.com/RafaelPires999/PDV_Mercado.git
   ```
2. Abra o arquivo `PDV_Mercado.sln` no **Visual Studio**.
3. Compile e execute (`Ctrl + F5`).
4. No primeiro acesso, cadastre o usuário administrador.

> O sistema foi feito para **Windows** (usa `system("cls")`).

## 👨‍💻 Autor

**Rafael Aguiar Pires** — estudante de Análise e Desenvolvimento de Sistemas na FAI (Adamantina-SP)
[GitHub](https://github.com/RafaelPires999)
