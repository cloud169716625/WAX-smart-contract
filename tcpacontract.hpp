/**
 * @name tcpa
 * @dev detect that one transfer EOS token to account deployed this contract
 *     and record its amount in the multi-index table
 * @version eosio.cdt v1.4.1
**/

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>

namespace eosio {

using std::string;

class [[eosio::contract]] tcpacontract : public eosio::contract {
    public:
        /**
         * Constructor
        **/

        tcpacontract( name receiver, name code, datastream<const char*> ds ):
            contract::contract( receiver, code, ds ),
            total_deposit_table( receiver, receiver.value ) {}

        /**
         * Struct
        **/

        /// amount which sender account transfer to eosio.token contract
        struct [[eosio::table]] account {
            asset balance;

            uint64_t primary_key() const { return balance.symbol.code().raw(); }
        };

        /**
         * Multi Index
        **/

        using account_index = eosio::multi_index< name("accounts"), account >;
        using deposit_index = eosio::multi_index< name("deposit"), account >;
        using total_deposit_index = eosio::multi_index< name("totaldeposit"), account >;

        /**
         * Public Function
        **/

        [[eosio::action]] void      deposit( name from, name to, asset quantity, string memo );
        [[eosio::action]] void     withdraw( name user, asset quantity );

    private:
        /**
         * Multi Index Table
        **/

        total_deposit_index total_deposit_table;

        /**
         * Public Function
        **/

        void add_deposit( name user, asset quantity );
        void sub_deposit( name user, asset quantity );
        void transfer_to( name user, asset quantity, string message );
        asset get_total_deposit( symbol sym );
        asset get_current_balance( symbol sym );
};

} // namespace eosio